#pragma once

namespace CrewEvents
{
	enum Event
	{
		PLAYER_TAKE_CONTROL,
		PLAYER_RELEASE_CONTROL,
		GUNNER_ENEMY_HIT,
		GUNNER_ENEMY_BURN,
		GUNNER_ARMOR_PENETRATION,
		GUNNER_ENEMY_DESTROYED,
		GUNNER_GETTING_DAMAGE
	};
}

class SubscriptionBase
{
public:
	virtual CrewEvents::Event GetEventName() const = 0;

	virtual void Invoke() const = 0;
};

template <typename ListenerType> class Subscription : public SubscriptionBase
{
private:
	virtual CrewEvents::Event GetEventName(void) const
	{
		return this->event;
	}

	virtual void Invoke(void) const
	{
		(this->listener->*this->action)();
	}

public:
	CrewEvents::Event event;
	ListenerType* listener;
	void(ListenerType ::* action )(void);

	Subscription(CrewEvents::Event event, ListenerType* listener, void(ListenerType::*action)(void))
	{
		this->event = event;
		this->listener = listener;
		this->action = action;
	};
};

class EventDispatcher
{
private:
	ed::vector<SubscriptionBase*> subscriptions;

protected:
	void DispatchEvent(const CrewEvents::Event event) const;

public:
	// template <typename ListenerType> void AddEventListener(ed::string, ListenerType*, void (ListenerType::*)(void));
	template <typename ListenerType> void RemoveEventListener(CrewEvents::Event, ListenerType*, void (ListenerType::*)(void));

	template <typename ListenerType> void AddEventListener(CrewEvents::Event event, ListenerType* listener, void (ListenerType::*action)(void))
	{
		Subscription<ListenerType>* subscription = new Subscription<ListenerType>(event, listener, action);

		this->subscriptions.push_back(subscription);
	}

	template <typename ListenerType> void EventDispatcher::RemoveEventListener (CrewEvents::Event event, ListenerType* listener, void (ListenerType::*action)(void))
	{
		for (int i = 0; i < this->subscriptions.size(); ++i)
		{
			Subscription<ListenerType>* subscription = dynamic_cast<Subscription<ListenerType>*>(this->subscriptions[i]);

			if (subscription && subscription->listener == listener && subscription->action == action)
				this->subscriptions.erase(this->subscriptions.begin() + i);
		}
	}
};