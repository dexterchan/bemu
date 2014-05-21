//------------------------------------------------------------------------------------------------
// <copyright project="BEmu_cpp" file="src/IntradayBarRequest/IntradayBarRequest.cpp" company="Jordan Robinson">
//     Copyright (c) 2013 Jordan Robinson. All rights reserved.
//
//     The use of this software is governed by the Microsoft Public License
//     which is included with this distribution.
// </copyright>
//------------------------------------------------------------------------------------------------

#include "IntradayBarRequest/IntradayBarRequest.h"

#include "IntradayBarRequest/IntradayBarRequestElementInt.h"
#include "IntradayBarRequest/IntradayBarRequestElementBool.h"
#include "IntradayBarRequest/IntradayBarRequestElementTime.h"
#include "IntradayBarRequest/IntradayBarRequestElementString.h"
#include "IntradayBarRequest/IntradayBarRequestElementStringArray.h"

#include "BloombergTypes/Service.h"


namespace BEmu
{
	namespace IntradayBarRequest
	{
		IntradayBarRequest::IntradayBarRequest(const Service& svc)
		{
			this->_eventTypes = 0;
			this->_security = 0;
			this->_dtStart = 0;
			this->_dtEnd = 0;
			this->_intervalInMinutes = 0;
			this->_gapFillInitialBar = 0;
			this->_returnEids = 0;
			this->_adjustmentNormalElement = 0;
			this->_adjustmentAbnormalElement = 0;
			this->_adjustmentSplitElement = 0;
			this->_adjustmentFollowDPDF = 0;

			this->_service = svc;
			this->_eventTypes = new IntradayBarRequestElementStringArray("TBD");
			this->_requestType = RequestPtr::intradayBar;
		}

		IntradayBarRequest::~IntradayBarRequest()
		{
			delete this->_eventTypes;
			this->_eventTypes = 0;

			delete this->_security;
			this->_security = 0;

			delete this->_dtStart;
			this->_dtStart = 0;

			delete this->_dtEnd;
			this->_dtEnd = 0;

			delete this->_intervalInMinutes;
			this->_intervalInMinutes = 0;

			delete this->_gapFillInitialBar;
			this->_gapFillInitialBar = 0;

			delete this->_returnEids;
			this->_returnEids = 0;

			delete this->_adjustmentNormalElement;
			this->_adjustmentNormalElement = 0;

			delete this->_adjustmentAbnormalElement;
			this->_adjustmentAbnormalElement = 0;

			delete this->_adjustmentSplitElement;
			this->_adjustmentSplitElement = 0;

			delete this->_adjustmentFollowDPDF;
			this->_adjustmentFollowDPDF = 0;
		}

		std::vector<Datetime>* IntradayBarRequest::getDateTimes() const
		{
			//dtStart, dtEnd, and interval are required
			if(this->_dtStart == 0 || this->_dtEnd == 0 || this->_intervalInMinutes == 0)
				throw requestEx;

			int intervalInMinutes = this->_intervalInMinutes->getInt();
			if (intervalInMinutes < 1 || intervalInMinutes > 1440) //if less than one, the loop below will never terminate
				throw requestEx;
			
			Datetime dtStart = this->_dtStart->getDatetime();
			Datetime dtEnd = this->_dtEnd->getDatetime();

			if(dtStart >= dtEnd) //dtStart must be before dtEnd
				throw requestEx;

			std::vector<Datetime>* result = new std::vector<Datetime>();

			Datetime dtLoop(dtStart.year(), dtStart.month(), dtStart.day(), dtStart.hours(), dtStart.minutes(), dtStart.seconds());
			while(dtLoop < dtEnd)
			{
				result->push_back(Datetime(dtLoop));
				dtLoop.addMinutes(this->_intervalInMinutes->getInt());
			}

			return result;
		}

		bool IntradayBarRequest::hasStartDate() const
		{
			return this->_dtStart != 0;
		}

		bool IntradayBarRequest::hasEndDate() const
		{
			return this->_dtEnd != 0;
		}

		Datetime IntradayBarRequest::getDtStart() const
		{
			return this->_dtStart->getDatetime();
		}

		Datetime IntradayBarRequest::getDtEnd() const
		{
			return this->_dtEnd->getDatetime();
		}

		const Service IntradayBarRequest::getService() const
		{
			return this->_service;
		}

		const std::string& IntradayBarRequest::security() const
		{
			return this->_security->security();
		}

		void IntradayBarRequest::set(const char* name, const char* value)
		{
			if(strncmp(name, "security", 9) == 0)
			{
				this->_security = new IntradayBarRequestElementString(name, value);
			}
			else if(strncmp(name, "eventType", 10) == 0)
			{
				this->_eventTypes->addValue(value);
			}
			else
				throw requestEx;
		}

		void IntradayBarRequest::set(const char* name, const Datetime& elementValue)
		{
			if(strncmp(name, "startDateTime", 14) == 0)
			{
				this->_dtStart = new IntradayBarRequestElementTime(name, elementValue);
			}
			else if(strncmp(name, "endDateTime", 12) == 0)
			{
				this->_dtEnd = new IntradayBarRequestElementTime(name, elementValue);
			}
			else
				throw requestEx;
		}

		void IntradayBarRequest::set(const char* name, int elementValue)
		{
			if(strncmp(name, "interval", 9) == 0)
			{
				this->_intervalInMinutes = new IntradayBarRequestElementInt(name, elementValue);
			}
			else
				throw requestEx;
		}

		void IntradayBarRequest::set(const char* name, bool elementValue)
		{
			if(strncmp(name, "gapFillInitialBar", 18) == 0)
			{
				this->_gapFillInitialBar = new IntradayBarRequestElementBool(name, elementValue);
			}
			else if(strncmp(name, "returnEids", 11) == 0)
			{
				this->_returnEids = new IntradayBarRequestElementBool(name, elementValue);
			}
			else if(strncmp(name, "adjustmentNormal", 17) == 0)
			{
				this->_adjustmentNormalElement = new IntradayBarRequestElementBool(name, elementValue);
			}
			else if(strncmp(name, "adjustmentAbnormal", 19) == 0)
			{
				this->_adjustmentAbnormalElement = new IntradayBarRequestElementBool(name, elementValue);
			}
			else if(strncmp(name, "adjustmentSplit", 16) == 0)
			{
				this->_adjustmentSplitElement = new IntradayBarRequestElementBool(name, elementValue);
			}
			else if(strncmp(name, "adjustmentFollowDPDF", 21) == 0)
			{
				this->_adjustmentFollowDPDF = new IntradayBarRequestElementBool(name, elementValue);
			}
			else
				throw requestEx;
		}
	}
}