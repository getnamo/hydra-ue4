class HydraUtilityTimer
{
	int64 TickTime = 0;
	int64 TockTime = 0;
public:
	HydraUtilityTimer()
	{
		tick();
	}

	double unixTimeNow()
	{
		FDateTime timeUtc = FDateTime::UtcNow();
		return timeUtc.ToUnixTimestamp() * 1000 + timeUtc.GetMillisecond();
	}

	void tick()
	{
		TickTime = unixTimeNow();
	}

	//return time elapsed in seconds
	float tock()
	{
		TockTime = unixTimeNow();
		return float(TockTime - TickTime) / 1000.f;
	}
};

//UE v4.6 IM event wrappers
bool EmitKeyUpEventForKey(FKey key, int32 user, bool repeat)
{
	FKeyEvent KeyEvent(key, FSlateApplication::Get().GetModifierKeys(), user, repeat, 0, 0);
	return FSlateApplication::Get().ProcessKeyUpEvent(KeyEvent);
}

bool EmitKeyDownEventForKey(FKey key, int32 user, bool repeat)
{
	FKeyEvent KeyEvent(key, FSlateApplication::Get().GetModifierKeys(), user, repeat, 0, 0);
	return FSlateApplication::Get().ProcessKeyDownEvent(KeyEvent);
}

bool EmitAnalogInputEventForKey(FKey key, float value, int32 user, bool repeat)
{
	FAnalogInputEvent AnalogInputEvent(key, FSlateApplication::Get().GetModifierKeys(), user, repeat, 0, 0, value);
	return FSlateApplication::Get().ProcessAnalogInputEvent(AnalogInputEvent);
}

//Function used for consistent conversion to input mapping basis
float MotionInputMappingConversion(float AxisValue) {
	return FMath::Clamp(AxisValue / 200.f, -1.f, 1.f);
}