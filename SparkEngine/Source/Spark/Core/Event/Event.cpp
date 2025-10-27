#include "SparkPCH.h"

#include "Spark/Core/ThreadPool.h"
#include "Spark/Core/Launch.h"
#include "Spark/Core/Event/EventCategory.h"

#include "Spark/Core/Event/Event.h"

namespace Spark
{
    // 在类外初始化静态成员
    std::unordered_map<EventCategory, std::vector<std::string>> EventBus::category_to_event_names_;
    std::unordered_map<std::string, EventDispatcherBase*> EventBus::dispatcher_map_;
    std::mutex EventBus::category_mutex_;
    std::mutex EventBus::dispatcher_mutex_;
}
