#pragma once
#include <unordered_map>
#include <string>
#include "Delegate.h"

// Used to bind an action to pressed or released
enum InputEvent
{
	IE_Pressed,
	IE_Released
};

// Action delegate takes no parameters
typedef DelegateNoParamsPtr ActionDelegatePtr;

// Axis delegate takes a single float parameter
typedef std::shared_ptr<DelegateBase<float>> AxisDelegatePtr;

class InputManager
{
public:
	// These functions are called from Game when input events are received
	void HandleKeyPressed(int key);
	void HandleKeyReleased(int key);

	// Creates an action mapping with the specified name and key
	void AddActionMapping(const std::string& name, int key);

	// Creates an axis mapping with the specified name and positive/negative keys
	void AddAxisMapping(const std::string& name, int positiveKey, int negativeKey);

	// Given the name of an action, binds a member function to be triggered by it
	// Parameters:
	// - Name of action
	// - InputEvent type (either IE_Pressed or IE_Released)
	// - Pointer to object (usually this)
	// - Pointer to member function
	template <typename T>
	void BindAction(const std::string& name, InputEvent event,
		T* obj, void (T::*func)())
	{
		// Template magic to construct the delegate pointer and pass it
		// to BindActionInternal
		ActionDelegatePtr delegate = std::make_shared<Delegate<T>>(obj, func);
		BindActionInternal(name, event, delegate);
	}

	// Given the name of an axis, binds a member function to be triggered by it
	// Parameters:
	// - Name of axis
	// - Pointer to object (usually this)
	// - Pointer to member function
	template <typename T>
	void BindAxis(const std::string& name, T* obj, void (T::*func)(float))
	{
		// Template magic to construct the delegate pointer and pass it
		// to BindActionInternal
		AxisDelegatePtr delegate = std::make_shared<Delegate<T, float>>(obj, func);
		BindAxisInternal(name, delegate);
	}

	// Parses input mappings from the specified file
	// See Config/Mappings.itpconfig as an example of the JSON format
	void ParseMappings(const char* fileName);
private:
	void BindActionInternal(const std::string& name, InputEvent event, 
		ActionDelegatePtr delegate);
	void BindAxisInternal(const std::string& name, AxisDelegatePtr delegate);

	// Struct used for a mapping of a single action
	struct ActionMapping
	{
		// Name of this mapping
		std::string mName;
		// Key associated with this mapping
		int mKey;
		// Delegate to trigger when action is "pressed"
		ActionDelegatePtr mPressedDelegate;
		ActionDelegatePtr mReleasedDelegate;
	};

	typedef std::shared_ptr<ActionMapping> ActionMappingPtr;
	
	// Struct used for mapping an axis to two keys
	struct AxisMapping
	{
		// Name of this mapping
		std::string mName;
		// Key associated with +1.0f on the axis
		int mPositiveKey;
		// Key associated with -1.0f on the axis
		int mNegativeKey;
		// Current value of this axis
		float mValue;
		// Delegate to trigger when value updates
		AxisDelegatePtr mDelegate;
	};

	typedef std::shared_ptr<AxisMapping> AxisMappingPtr;

	// These map string names to either action or axis mappings
	std::unordered_map<std::string, ActionMappingPtr>
		mNameToActionMap;
	
	std::unordered_map<std::string, AxisMappingPtr>
		mNameToAxisMap;

	// These map keys to their corresponding action or axis mappings
	std::unordered_map<int, ActionMappingPtr>
		mKeyToActionMap;

	std::unordered_map<int, AxisMappingPtr>
		mKeyToAxisMap;
};
