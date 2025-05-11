#pragma once
#include <functional>
#include <vector>


using EventHandlerID = size_t;

template<typename... Args>
class Event
{
public:
	using EventHandler = std::function<void(Args...)>;
	
	EventHandlerID subscribe(EventHandler handler)
	{
		m_handlers.push_back(handler);
		return m_handlers.size() - 1;
	}

	void unsubscribe(EventHandlerID handlerID)
	{
		m_handlers.erase(m_handlers.begin() + handlerID);
	}

	void invoke(Args... args)
	{
		for (const auto& handler : m_handlers)
		{
			handler(args...);
		}
	}
private:
	std::vector<EventHandler> m_handlers = {};
};