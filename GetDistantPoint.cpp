#include<iostream>
#include<tuple>
#include<set>
#include<vector>
#include<fstream>

int GetVectorIDFromTwoDimensionalID(int GridSize, int xid, int yid);
std::pair<int,int> GetTwoDimensionalIDFromVectorID(int GridSize, int VectorID);

void ExploreConnectedRegions(int GridSize, int CurrentID, int DistinctRegionID,  const std::set<std::tuple<int,int,int,int>> &SetOfWalls, std::vector<bool> &vExploredStatus, std::vector<int> &vDistinctRegionState);


bool GetLeftID(int GridSize, int CurrentID, const std::vector<bool> &vExploredState, const std::set<std::tuple<int,int,int,int>> &SetOfWalls, int &NextID);


bool GetRightID(int GridSize, int CurrentID, const std::vector<bool> &vExploredState, const std::set<std::tuple<int,int,int,int>> &SetOfWalls, int &NextID);


bool GetBottomID(int GridSize, int CurrentID, const std::vector<bool> &vExploredState, const std::set<std::tuple<int,int,int,int>> &SetOfWalls, int &NextID);


bool GetTopID(int GridSize, int CurrentID, const std::vector<bool> &vExploredState, const std::set<std::tuple<int,int,int,int>> &SetOfWalls, int &NextID);


bool ReadInput(std::string sFileName, int GridSize, int NoOfWalls, int NoOfPoints, std::set<std::tuple<int,int,int,int>> &SetOfWalls, std::set<std::pair<int,int>> &SetOfPoints);

void PrintExploredStatus(int GridSize, const std::vector<int> &vDistinctRegionState);



int main(int argc, char *argv[])
{


  if(argc != 2)
  {
     std::cout << "Use the binary as ./getDistantPoint PathToInputFile" << std::endl;
     return 0;
  }

  std::string sInputFileName(argv[1]);
  int GridSize;
  int NoOfWalls;
  int NoOfPoints;

  std::set<std::tuple<int,int,int,int>> SetOfWalls;
  std::set<std::pair<int,int>> SetOfPoints;

  ReadInput(sInputFileName,GridSize,NoOfWalls,NoOfPoints,SetOfWalls,SetOfPoints);  

  std::vector<int> vDistinctRegionState(GridSize*GridSize,0);
  std::vector<bool> vExploredStatus(GridSize*GridSize,false);
  
  int NoOfDistinctRegion = 0;

 
     for(int i = 0; i < GridSize*GridSize; ++i)
     {
       
        if(!vExploredStatus[i])
         {
         ++NoOfDistinctRegion;
          ExploreConnectedRegions(GridSize,i,NoOfDistinctRegion,SetOfWalls,vExploredStatus,vDistinctRegionState);
         }
     }

   PrintExploredStatus(GridSize,vDistinctRegionState);

  return 0;

}


bool ReadInput(std::string sFileName, int GridSize, int NoOfWalls, int NoOfPoints, std::set<std::tuple<int,int,int,int>> &SetOfWalls, std::set<std::pair<int,int>> &SetOfPoints)
{

  std::ifstream reader(sFileName);

  if(!reader.is_open())
     {
     std::cout << "Error reading the input File " << std::endl;
     return false;
     }


 reader >> GridSize >> NoOfWalls >> NoOfPoints;

 int wallCount = 0;
 for(int i = 0; i < NoOfWalls; ++i)
  {
    int xid1, yid1, xid2, yid2;
    reader >> xid1 >> xid2 >> yid1 >> yid2;
    SetOfWalls.insert(std::make_tuple(xid1-1,yid1-1,xid2-1,yid2-1)); 
    ++wallCount; 
 }

  if(wallCount != NoOfWalls)
   return false;

  int pointCount = 0;
  for(int i = 0; i < NoOfPoints; ++i)
   {
     int xid, yid;
     reader >> xid >> yid;
     SetOfPoints.insert(std::make_pair(xid,yid));
     ++pointCount;
   }

  if(pointCount != NoOfPoints)
   return false;

  return true;

}



int GetVectorIDFromTwoDimensionalID(int GridSize, int xid, int yid)
{
   return (yid*GridSize + xid);
}


std::pair<int,int> GetTwoDimensionalIDFromVectorID(int GridSize, int VectorID)
{
   int yid = VectorID/GridSize;
   int xid = VectorID%GridSize;
   return std::make_pair(xid,yid);
}



void ExploreConnectedRegions(int GridSize, int CurrentID, int DistinctRegionID,  const std::set<std::tuple<int,int,int,int>> &SetOfWalls, std::vector<bool> &vExploredStatus, std::vector<int> &vDistinctRegionState)
{


  vExploredStatus[CurrentID] = true;
  vDistinctRegionState[CurrentID] = DistinctRegionID;  

  int NextID;  

  if(GetLeftID(GridSize,CurrentID,vExploredStatus,SetOfWalls,NextID))
  {
	  ExploreConnectedRegions(GridSize,NextID,DistinctRegionID,SetOfWalls,vExploredStatus,vDistinctRegionState);
  }

  if(GetRightID(GridSize,CurrentID,vExploredStatus,SetOfWalls,NextID))
  {
	  ExploreConnectedRegions(GridSize,NextID,DistinctRegionID,SetOfWalls,vExploredStatus,vDistinctRegionState);
  }

  if(GetBottomID(GridSize,CurrentID,vExploredStatus,SetOfWalls,NextID))
  {
	  ExploreConnectedRegions(GridSize,NextID,DistinctRegionID,SetOfWalls,vExploredStatus,vDistinctRegionState);
  }

  if(GetTopID(GridSize,CurrentID,vExploredStatus,SetOfWalls,NextID))
  {
	  ExploreConnectedRegions(GridSize,NextID,DistinctRegionID,SetOfWalls,vExploredStatus,vDistinctRegionState);
  }


   return;

}


bool GetLeftID(int GridSize, int CurrentID, const std::vector<bool> &vExploredState, const std::set<std::tuple<int,int,int,int>> &SetOfWalls, int &NextID)
{

    std::pair<int,int> TwoDimensionalCurrentID = GetTwoDimensionalIDFromVectorID(GridSize, CurrentID);
    int xid = TwoDimensionalCurrentID.first;
    int yid = TwoDimensionalCurrentID.second;    

    if ((xid <= 0))
     return false;
  
    NextID =  GetVectorIDFromTwoDimensionalID(GridSize,xid-1,yid);     
     
    if(vExploredState[NextID])
     return false;

    std::tuple<int,int,int,int> wallID1(xid,yid,xid-1,yid);
    std::tuple<int,int,int,int> wallID2(xid-1,yid,xid,yid);
 
    auto search1 = SetOfWalls.find(wallID1);
    auto search2 = SetOfWalls.find(wallID2);

    if(search1 != SetOfWalls.end() || search2 != SetOfWalls.end())
     return false;

   
    return true;
   
}


bool GetRightID(int GridSize, int CurrentID, const std::vector<bool> &vExploredState, const std::set<std::tuple<int,int,int,int>> &SetOfWalls, int &NextID)
{

    std::pair<int,int> TwoDimensionalCurrentID = GetTwoDimensionalIDFromVectorID(GridSize, CurrentID);
    int xid = TwoDimensionalCurrentID.first;
    int yid = TwoDimensionalCurrentID.second;    

    if ((xid >= GridSize-1))
     return false;
  
    NextID =  GetVectorIDFromTwoDimensionalID(GridSize,xid+1,yid);     
     
    if(vExploredState[NextID])
     return false;

    std::tuple<int,int,int,int> wallID1(xid,yid,xid+1,yid);
    std::tuple<int,int,int,int> wallID2(xid+1,yid,xid,yid);
 
    auto search1 = SetOfWalls.find(wallID1);
    auto search2 = SetOfWalls.find(wallID2);

    if(search1 != SetOfWalls.end() || search2 != SetOfWalls.end())
     return false;

   
    return true;
   
}

bool GetBottomID(int GridSize, int CurrentID, const std::vector<bool> &vExploredState, const std::set<std::tuple<int,int,int,int>> &SetOfWalls, int &NextID)
{

    std::pair<int,int> TwoDimensionalCurrentID = GetTwoDimensionalIDFromVectorID(GridSize, CurrentID);
    int xid = TwoDimensionalCurrentID.first;
    int yid = TwoDimensionalCurrentID.second;    

    if ((yid <= 0))
     return false;
  
    NextID =  GetVectorIDFromTwoDimensionalID(GridSize,xid,yid-1);     
     
    if(vExploredState[NextID])
     return false;

    std::tuple<int,int,int,int> wallID1(xid,yid,xid,yid-1);
    std::tuple<int,int,int,int> wallID2(xid,yid-1,xid,yid);
 
    auto search1 = SetOfWalls.find(wallID1);
    auto search2 = SetOfWalls.find(wallID2);

    if(search1 != SetOfWalls.end() || search2 != SetOfWalls.end())
     return false;

   
    return true;
   
}

bool GetTopID(int GridSize, int CurrentID, const std::vector<bool> &vExploredState, const std::set<std::tuple<int,int,int,int>> &SetOfWalls, int &NextID)
{

    std::pair<int,int> TwoDimensionalCurrentID = GetTwoDimensionalIDFromVectorID(GridSize, CurrentID);
    int xid = TwoDimensionalCurrentID.first;
    int yid = TwoDimensionalCurrentID.second;    

    if ((yid >= GridSize-1))
     return false;
  
    NextID =  GetVectorIDFromTwoDimensionalID(GridSize,xid,yid+1);     
     
    if(vExploredState[NextID])
     return false;

    std::tuple<int,int,int,int> wallID1(xid,yid,xid,yid+1);
    std::tuple<int,int,int,int> wallID2(xid,yid+1,xid,yid);
 
    auto search1 = SetOfWalls.find(wallID1);
    auto search2 = SetOfWalls.find(wallID2);

    if(search1 != SetOfWalls.end() || search2 != SetOfWalls.end())
     return false;

   
    return true;
   
}





void PrintExploredStatus(int GridSize, const std::vector<int> &vDistinctRegionState)
{
  
   for(int i = 0; i < GridSize; ++i)
    {
     for(int j = 0; j < GridSize; ++j)
     std::cout << vDistinctRegionState[i*GridSize + j] << " ";

     std::cout << std::endl;


    }
  

}
