#include<iostream>
	#include<fstream>
	#include<vector>
	#include<algorithm>
        #include<cmath>
	#include<math.h>

	bool ReadInput(const std::string &sFileName, int &NoOfEvents, std::vector<int> &vDistanceEvents, std::vector<int>&vTimeEvents);

	bool GetNextClosestEvent(const std::vector<int> &DistanceEvents, double currentDistance, int &nextDistance, int &NoOfRept);


	bool IsDistanceEventCloserThanTime(double currentDistance, double nextDistance, double currentTime, double nextTime, int evenCount);


	bool TwoEventsOccurSim(double currentDistance, double nextDistance, double currentTime, double nextTime, int evenCount);



	int main(int argc, char *argv[])
	{
	  
	 if(argc != 2)
	 {
	  std::cout << "Use the executable as ./getTotalTime PathToInputFile" << std::endl;
	  return 1;
	 }

	 std::string sFileName(argv[1]);
	 int NoOfEvents;

	 //Vector of Distance Events
	  std::vector<int> vDistanceEvents;
	 //Vector of Time Events
	  std::vector<int> vTimeEvents;
	 
	  if(!ReadInput(sFileName,NoOfEvents,vDistanceEvents,vTimeEvents))
	   return 1;

	  
	  std::sort(vDistanceEvents.begin(),vDistanceEvents.end());
	  std::sort(vTimeEvents.begin(), vTimeEvents.end());

	  for(int i = 0; i < vDistanceEvents.size() ; ++i)
	    std::cout << "Input Distance Event = " << vDistanceEvents[i] << std::endl;

	  for(int i = 0; i < vTimeEvents.size(); ++i)
	   std::cout << "Input Time Evnet = " << vTimeEvents[i] << std::endl; 
	 

	  double currentDistance = 0.0;
	  double currentTime = 0.0;
	  double currentSpeed = 1.0;
	  double distanceToCover = 1000.0;
	  int eventCount = 1;

	 while (currentDistance < distanceToCover)
	 {
	  

	  int nextDistance;
	  int nextTime;
	  bool bFoundDistanceEvent(false);
	  bool bFoundTimeEvent(false);
          int NoOfRepForDistance = 0;
          int NoOfRepForTime = 0;

          //Get Next closest Distance Event
	  if(GetNextClosestEvent(vDistanceEvents, currentDistance, nextDistance, NoOfRepForDistance))
	   {
	   //std::cout << "Distance Event = " << (int)nextDistance << std::endl;
	   bFoundDistanceEvent = true;   
	   }
	  
           //Get Next Closest Time Event
	  if(GetNextClosestEvent(vTimeEvents, currentTime, nextTime, NoOfRepForTime))
	  { 
	   //std::cout << "Time Event = " << (int)nextTime << std::endl;
	   bFoundTimeEvent = true;
	  }
	  
	  bool bChooseDistanceEvent(false);
	  bool bChooseTimeEvent(false);
	  bool bTwoEventsOccur(false);

	  if((!bFoundTimeEvent) && (bFoundDistanceEvent))
	   bChooseDistanceEvent = true;

	  if((!bFoundDistanceEvent) && (bFoundTimeEvent))
	   bChooseTimeEvent = true;
	  
	  if(bFoundDistanceEvent && bFoundTimeEvent)
	   {
	  
          if(IsDistanceEventCloserThanTime(currentDistance,nextDistance,currentTime,nextTime,eventCount))
	   {
	   //std::cout << "current Speed = " << eventCount << std::endl;
	   //std::cout << "Distance Event = " << nextDistance << std::endl;
	   bChooseDistanceEvent = true;
	   }
	   else
	   {
	   //std::cout << "Event Count = " << eventCount << std::endl;
	   //std::cout << "Time Event = " << nextTime << "  Distance = " << currentDistance << std::endl;
	   bChooseTimeEvent = true;
	   }
	   }


	  if(bChooseDistanceEvent)
	   {
	    currentTime = currentTime + ((nextDistance - currentDistance)*eventCount);
	    currentDistance = nextDistance;
	    eventCount = eventCount + NoOfRepForDistance;
	    currentSpeed = 1.0/eventCount;
	   }
	  
	  if(bChooseTimeEvent)
	  {
	   currentDistance = currentDistance  + (nextTime - currentTime)*1.0/eventCount;
	   currentTime = nextTime;
	   eventCount = eventCount + NoOfRepForTime;
	   currentSpeed = 1.0/eventCount;
	  }
	 
	  if((!bChooseDistanceEvent) && (!bChooseTimeEvent))
	  {
	    currentTime = currentTime + ((distanceToCover - currentDistance)*eventCount);
	    currentDistance = distanceToCover;
	  }


	}
	  
	 std::cout << "Time to complete the journey = " << (int)round(currentTime) << std::endl;
	 return 0;

	}


	bool IsDistanceEventCloserThanTime(double currentDistance, double nextDistance, double currentTime, double nextTime, int eventCount)
	{

	 return ( ((nextDistance - currentDistance)) < ((nextTime - currentTime)*1.0/eventCount));

	}



bool TwoEventsOccurSim(double currentDistance, double nextDistance, double currentTime, double nextTime, int eventCount)
{

 return ( fabs(((nextDistance - currentDistance)) - ((nextTime - currentTime)*1.0/eventCount)) < 1e-1);

}




bool GetNextClosestEvent(const std::vector<int> &DistanceEvents, double currentDistance, int &nextDistance, int &NoOfRept)
{
  auto nextDistanceIter = std::upper_bound(DistanceEvents.begin(), DistanceEvents.end(), static_cast<int>(currentDistance));
  bool bFoundNextClosestEvent = false;
   
  if(nextDistanceIter < DistanceEvents.end())
  {
  nextDistance = *nextDistanceIter;
  bFoundNextClosestEvent = true;
  }
  
  NoOfRept = 0;
  
  if(nextDistanceIter < DistanceEvents.end())
  {
   
    while(*nextDistanceIter == nextDistance)
    {
      ++NoOfRept;
      ++nextDistanceIter;
      if(nextDistanceIter == DistanceEvents.end())
      break; 
   }  
 
  }

  if(bFoundNextClosestEvent)
  std::cout << "No of Repetition for Event = " << nextDistance << " is " << NoOfRept << std::endl;
  return bFoundNextClosestEvent; 
 
}



bool ReadInput(const std::string &sFileName, int &NoOfEvents, std::vector<int> &vDistanceEvents, std::vector<int> &vTimeEvents)
{

  std::ifstream reader(sFileName);
  
  if(!reader.is_open())
   {
    std::cout << "Unable to open the input file" << std::endl;
    return false;
   }

   reader >> NoOfEvents;

   char EventType;
   int EventValue;
   for(int i = 0; i < NoOfEvents; ++i)
   {
     reader >> EventType >> EventValue;

     if(EventType == 'D')
      vDistanceEvents.push_back(EventValue);
      
    if(EventType == 'T')
      vTimeEvents.push_back(EventValue);
     //std::cout << "Event = " << EventType << " " << EventValue << std::endl; 
  }
   

 return true;
}
