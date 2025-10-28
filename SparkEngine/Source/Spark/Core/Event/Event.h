#pragma once

namespace Spark
{
    // 事件基类
    class Event
    {
    public:
        virtual ~Event() = default;
        static const char* GetEventName() { return "UnknownEvent"; }
        static EventCategory GetEventCategory() { return EventCategory::EC_None; }
    };

    // 事件分发器基类
    class EventDispatcherBase
    {
    public:
        virtual ~EventDispatcherBase() = default;
        virtual void Publish(const Event& event) = 0;
        virtual void PublishAsync(const Event& event) = 0;
        virtual void RemoveAllListeners() = 0;
    };

    // 事件分发器模板类
    template <typename T>
    class EventDispatcher : public EventDispatcherBase
    {
    public:
        using Callback = std::function<void(const T&)>;
        using SubscriptionToken = int; // 简化：使用int作为令牌

        SubscriptionToken AddListener(Callback callback)
        {
            std::lock_guard<std::mutex> lock(mutex_);
            int id = next_id_++;
            callbacks_.emplace(id, std::move(callback));
            return id;
        }

        void RemoveListener(SubscriptionToken token)
        {
            std::lock_guard<std::mutex> lock(mutex_);
            callbacks_.erase(token);
        }

        void Publish(const T& event)
        {
            std::lock_guard<std::mutex> lock(mutex_);
            for (const auto& [id, callback] : callbacks_)
            {
                callback(event);
            }
        }

        void PublishAsync(const T& event)
        {
            std::lock_guard<std::mutex> lock(mutex_);
            for (const auto& [id, callback] : callbacks_)
            {
                ThreadPool::GetInstance().Enqueue([callback, event]{
                    callback(event);
                    });
            }
        }

        void RemoveAllListeners()
        {
            std::lock_guard<std::mutex> lock(mutex_);
            callbacks_.clear();
        }

        // 虚函数实现
        void Publish(const Event& event) override
        {
            if (const auto* typed_event = dynamic_cast<const T*>(&event))
            {
                Publish(*typed_event);
            }
        }

        void PublishAsync(const Event& event) override
        {
            if (const auto* typed_event = dynamic_cast<const T*>(&event))
            {
                PublishAsync(*typed_event);
            }
        }

    private:
        mutable std::mutex mutex_;
        std::unordered_map<int, Callback> callbacks_;
        int next_id_ = 0;
    };

    // 事件总线（单例模式）
    class EventBus
    {
    public:
        // 禁止实例化
        EventBus(const EventBus&) = delete;
        EventBus& operator=(const EventBus&) = delete;

        // 获取单例实例
        static EventBus& GetInstance()
        {
            static EventBus instance;
            return instance;
        }

        template <typename T>
        typename EventDispatcher<T>::SubscriptionToken AddListener(
            const std::function<void(const T&)>& callback
        )
        {
            static_assert(std::is_base_of<Event, T>::value, "Event type must inherit from Event");
            auto dispatcher = GetDispatcher<T>(T::GetEventName());
            RegisterEventCategory(T::GetEventName(), T::GetEventCategory());
            return dispatcher->AddListener(callback);
        }

        template <typename T>
        void Publish(const T& event)
        {
            static_assert(std::is_base_of<Event, T>::value, "Event type must inherit from Event");
            auto dispatcher = GetDispatcher<T>(T::GetEventName());
            dispatcher->Publish(event);
        }

        template <typename T>
        void PublishAsync(const T& event)
        {
            static_assert(std::is_base_of<Event, T>::value, "Event type must inherit from Event");
            auto dispatcher = GetDispatcher<T>(T::GetEventName());
            dispatcher->PublishAsync(event);
        }

        template <typename T>
        void RemoveListener(const typename EventDispatcher<T>::SubscriptionToken& token)
        {
            static_assert(std::is_base_of<Event, T>::value, "Event type must inherit from Event");
            auto dispatcher = GetDispatcher<T>(T::GetEventName());
            dispatcher->RemoveListener(token);
        }

        void RemoveAllListenersInCategory(EventCategory category)
        {
            std::lock_guard<std::mutex> lock(category_mutex_);
            if (category_to_event_names_.find(category) == category_to_event_names_.end())
            {
                return;
            }

            for (const auto& eventName : category_to_event_names_[category])
            {
                auto dispatcher = GetDispatcherByEventName(eventName);
                if (dispatcher)
                {
                    dispatcher->RemoveAllListeners();
                }
            }
        }

    private:
        // 私有构造函数
        EventBus() = default;

        template <typename T>
        EventDispatcher<T>* GetDispatcher(const std::string& eventName)
        {
            static std::mutex mutex;
            static std::unordered_map<std::string, std::unique_ptr<EventDispatcher<T>>> dispatchers;

            {
                std::lock_guard<std::mutex> lock(mutex);
                if (dispatchers.find(eventName) == dispatchers.end())
                {
                    dispatchers[eventName] = std::make_unique<EventDispatcher<T>>();
                    // 将分发器注册到全局映射
                    {
                        std::lock_guard<std::mutex> lock2(dispatcher_mutex_);
                        dispatcher_map_[eventName] = dispatchers[eventName].get();
                    }
                }
            }
            return dispatchers[eventName].get();
        }

        void RegisterEventCategory(const std::string& eventName, EventCategory category)
        {
            std::lock_guard<std::mutex> lock(category_mutex_);
            category_to_event_names_[category].push_back(eventName);
        }

        EventDispatcherBase* GetDispatcherByEventName(const std::string& eventName)
        {
            std::lock_guard<std::mutex> lock(dispatcher_mutex_);
            auto it = dispatcher_map_.find(eventName);
            return (it != dispatcher_map_.end()) ? it->second : nullptr;
        }

        // 静态成员初始化
        static std::unordered_map<EventCategory, std::vector<std::string>> category_to_event_names_;
        static std::unordered_map<std::string, EventDispatcherBase*> dispatcher_map_;
        static std::mutex category_mutex_;
        static std::mutex dispatcher_mutex_;
    };
}
