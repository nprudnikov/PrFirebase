// Copyright 2020-2021 Nikolay Prudnikov. All Rights Reserved.

package com.pr.firebase.performance;

import java.util.HashMap;
import java.util.Map;
import java.lang.reflect.Field;
import android.app.NativeActivity;
import com.google.firebase.perf.FirebasePerformance;
import com.google.firebase.perf.metrics.Trace;

public class PrFirebasePerformance {

	public PrFirebasePerformance(NativeActivity activity)
	{
		// Do nothing
	}

	private HashMap<Integer, Trace> traces = new HashMap<>();

	private Trace getTrace(int index)
	{
		Trace trace = traces.get(index);
		if (trace == null)
		{
			throw new RuntimeException("PrFirebasePerformance: Can't find trace by index: " + index);
		}

		return trace;
	}

	public void startTrace(int index, String identifier)
	{
		Trace trace = FirebasePerformance.getInstance().newTrace(identifier);
		trace.start();

		traces.put(index, trace);
	}

	public void stopTrace(int index)
	{
		Trace trace = getTrace(index);
		trace.stop();

		traces.remove(index);
	}

	public void incrementMetric(int index, String name, int value)
	{
		Trace trace = getTrace(index);
		trace.incrementMetric(name, value);
	}

	public void setMetric(int index, String name, int value)
	{
		Trace trace = getTrace(index);
		trace.putMetric(name, value);
	}

	private static Field attributesField = null;

	public void setAttribute(int index, String name, String value)
	{
		Trace trace = getTrace(index);

		// Slow regex check inside
		//trace.putAttribute(name, value);

		// Fast hack
		try
		{
			if (attributesField == null)
			{
				//attributesField = trace.getClass().getDeclaredField("customAttributesMap");
				attributesField = trace.getClass().getDeclaredField("attributes");
				attributesField.setAccessible(true);
			}
			
			Map<String, String> customAttributesMap = (Map<String, String>) attributesField.get(trace);
			customAttributesMap.put(name, value);
		}
		catch (Exception e)
		{
			throw new RuntimeException(e);
		}
	}

	public void removeAttribute(int index, String name)
	{
		Trace trace = getTrace(index);
		trace.removeAttribute(name);
	}
}
