//
// Copyright (C) 2016 David Eckhoff <david.eckhoff@fau.de>
//
// Documentation for these modules is at http://veins.car2x.org/
//
// SPDX-License-Identifier: GPL-2.0-or-later
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#include "MyCarApp.h"

using namespace veins;

Define_Module(veins::MyCarApp);
std::set<std::string> MyCarApp::setOfCars;
void MyCarApp::initialize(int stage)
{
    DemoBaseApplLayer::initialize(stage);
    if (stage == 0) {
    // Initializing members and pointers of your application goes here
    EV << "Initializing " << par("appName").stringValue() << std::endl;
    EV << "MyCarApp::myID is:: " << this->myId << std::endl;
    EV << "MyCarApp::getId is:: " << getId() << std::endl;
    newmsg = new cMessage("send Beacon");
    scheduleAt(simTime()+1, newmsg);
    //if (VehiclesAlreadyCounted == false)
    //{
        EV << "MyVeinsAppCar::getId is:: " << getId() << std::endl;
        totalCount++;
        VehiclesAlreadyCounted = true;
        traciVehicle->setSpeed(par("speed"));//"rsuspeed" is the speed comng from RSU0//
        EV << "MyAppCar::Speed:: " << par("speed").doubleValue() << std::endl;
    //}
    EV << "MyCarApp::initializing:: appName="<<par("appName").stringValue()
    <<", MyId="<<this->myId
    <<", Count After ="<<totalCount << std::endl;
    }
    else if (stage == 1) {
        // Initializing members that require initialized other modules goes here
        //else if(stage == DemoBaseApplLayer ::numIntstages())

         {
         traciVehicle->setSpeed(par("speed"));
         }

    }
}

void MyCarApp::finish()
{
    DemoBaseApplLayer::finish();
    EV << "MyCarApp::finish:: " << std::endl;
    EV << "Total number of nodes is:: " << totalCount << std::endl;
}

void MyCarApp::onBSM(DemoSafetyMessage* bsm)
{
    EV << "MyCarApp::onBSM: " << std::endl;
    // Your application has received a beacon message from another car or RSU code for handling the message goes here
}

void MyCarApp::onWSM(BaseFrame1609_4* wsm)
{
  EV << "MyCarApp::onWSM::" << std::endl;
  std::string currentCarsId = std::to_string(mac->getMACAddress()); //yehkyun??

  if(RSU* rsuwsm = dynamic_cast<RSU*>(wsm)) //msg received from RSU0
  {
      std::string setofCars = rsuwsm->getList();
      int rsuspeed = rsuwsm->getSpeed(); //assuming getSpeed() takes a set ID as an argument and returns the speed for that set
      EV <<"MyVeinsAppCar::onWSM::Slow down the vehicles:" << rsuspeed << std::endl;

      if(!setofCars.empty())
      {
          //check if current Id is present in the string sent from RSU
          if(setofCars.find(currentCarsId) != std::string::npos)
          {
              EV <<"MyCarApp::onWSM::speed received for vehicles:" << rsuspeed << endl;
              int currentSpeed = static_cast<int>(traciVehicle->getSpeed());
              EV << "MyCarApp::onWSM::new speed is the current speed:" <<" [" <<currentSpeed<< "]"<<endl;
              if(currentSpeed!= rsuspeed)
              {
                  EV << "MyCarApp::onWSM:: Change speed from [" << currentSpeed <<"] to -> [ "<< rsuspeed <<" ]"<< endl;
                  //traciVehicle->setSpeedMode(32); //uncommented


                  traciVehicle->setSpeed(rsuspeed);//"rsuspeed" is the speed comng from RSU0//
                  newmsg = new cMessage();
                  scheduleAt(simTime()+1, newmsg);
                  EV << "MyCarApp::onWSM:: received speed from the RSU0 [" << rsuspeed << "] " << "Car ID is " << getId() <<
                          " Change speed from current speed [" << currentSpeed <<"] to rsu speed-> [ "<< traciVehicle->getSpeed() <<" ]"<< endl;



                  currentSpeed = traciVehicle->getSpeed();

                  EV << "MyCarApp::onWSM::new speed is the current speed:" <<" [" <<currentSpeed<< "]"<<endl;
                  EV << "MyCarApp::onWSM::and new speed [ " << traciVehicle->getSpeed() << "]"<<endl;
                  EV << "MyCarApp::onWSM::size [" <<setofCars.size() << "] and new speed "<<endl;
              }
          }//If current vehicle (Id) exist in message from RSU, then change it's speed
      }   //if' line 79: ends here.
  }
}

void MyCarApp::handleSelfMsg(cMessage* msg)
{
    //DemoBaseApplLayer::handleSelfMsg();
    // this method is for self messages (mostly timers)
    // it is important to call the DemoBaseApplLayer function for BSM and WSM transmission
    EV << "MyCarApp::handleSelfMsg:: " << std::endl;
    EV << "MyCarApp::handleSelfMsg:: " << " Car ID is " << getId() << " my current speed is-> [ "<< traciVehicle->getSpeed() <<" ]"<< endl;
    //delete msg; //commented //02.10.23

}

void MyCarApp::handlePositionUpdate(cObject* obj)
{
   DemoBaseApplLayer::handlePositionUpdate(obj);
   //EV << "MyCarApp::handlePositionUpdate::Vehicle ID is: " << mac->getMACAddress() << std::endl;
   //EV << "MyCarApp::handlePositionUpdate:: getCommandInterface is: " <<mobility->getCommandInterface()<< ", traciVehicle (pointer var) is: " << mobility->getVehicleCommandInterface() << std::endl;

   EV << "MyCarApp::handlePositionUpdate::Speed::afterset " << par("speed").doubleValue() << std::endl;
   
   EV << "MyCarApp::handlePositionUpdate::Speed::afterget = " << traciVehicle->getSpeed() << std::endl;

   Coord Vehicle_position = mobility->getPositionAt(simTime()); // will be called each vehicle.
   double distance = cJunctioncoord.distance(Vehicle_position);
   EV << "MyCarApp::handlePositionUpdate::RoadId= " << traciVehicle->getRoadId() << ", Distance= " << distance << std::endl;
   VehicleId = mac->getMACAddress();
   std::string LaneId = traciVehicle->getLaneId();
   if(LaneId == "Entry_0" || LaneId == "Entry_1" || LaneId == "Entry_2")
   {
       EV << "MyCarApp::CheckLane::inside if:: before checkLane call::" << std::endl;
       CheckLane(traciVehicle, LaneId, VehicleId, distance,true);
       EV << "MyCarApp::CheckLane::inside if:: after checkLane call::" << std::endl;
   }
   else if(LaneId == "Exit_0" || LaneId == "Exit_1" || LaneId == "Exit_2")
   {
       EV << "MyCarApp::CheckLane::inside if:: before checkLane_exit call::" << std::endl;
       CheckLane(traciVehicle, LaneId, VehicleId, distance, false);
       EV << "MyCarApp::CheckLane::inside if:: after checkLane_exit call::" << std::endl;
   }
   /*
   else if((distance > 100 && distance <= 900 && traciVehicle->getLaneId() == "Entry_1"))
   {
       vehicleIsInRange = true;
       msg->setOutOfRange(false);
       std::string laneInfo = "Entry_1";
       msg->setLaneInfo(laneInfo.c_str());
       EV << "MyCarApp::handlePositionUpdate::vehicleId:[ "<< VehicleId << " ] from lane1 has ENTERED area of concern " << std::Endl;
   }
   
   else if(distance > 800 && traciVehicle->getRoadId() == "Entry_0")
   {}
   */
   printLanes(traci->getLaneIds(), "LaneIds");
}


void MyCarApp::CheckLane(TraCICommandInterface::Vehicle* traciVehicle, std::string LaneId, int VehicleId, double distance, bool IsEntryLane)
{
    EV << "MyCarApp::CheckLane:: " << std::endl;
    EV << "MyCarApp::CheckLane::RoadId= " << traciVehicle->getRoadId() << ", Distance= " << distance << std::endl;
    InitialMessage* msg = new InitialMessage(std::to_string(VehicleId).c_str());
    bool vehicleIsInRange; //bool 'status check' if the vehicle is in area of range. vehicleIsInRange == true If yes,
    if(IsEntryLane)
    {
        // Dividing the highway into two sections//area of concern: (100 - 800m)
        if(distance > 100 && distance <= 900)//left to right and in area of concern
        {
            EV << "MyCarApp::CheckLane::inside if:: condition area of concern check::" << std::endl;
            msg->setOutOfRange(false);
            msg->setTarget("MyRSUApp0");
            msg->setMsgID(1);
            const char* target = msg->getTarget();
            EV << "MyCarApp::CheckLane::inside IsEntryLane:: value of target is:" << target << std::endl;
        }
        else if(distance > 900) //out of area of concern
        {
            msg->setOutOfRange(true);
            msg->setTarget("MyRSUApp1.erase");
            msg->setMsgID(3);
            const char* target = msg->getTarget();
            traciVehicle->setSpeed(par("speed"));
        }
    }
    else
    {
        if(distance > 100 && distance <= 900) //right to left
        {
            EV << "MyCarApp::CheckLane::inside if:: right to left_area of concern check::" << std::endl;
            msg->setOutOfRange(false);
            msg->setTarget("MyRSUApp1"); //RSU1 act as a relay and it forwards msgs to "RSU0"
            msg->setMsgID(2);
        }
        else if(distance < 100)
        {
            msg->setOutOfRange(true);
            msg->setTarget("MyRSUApp0.erase");
            msg->setMsgID(4);
            traciVehicle->setSpeed(par("speed"));
        }
    }
    //msg->VehicleId()
    msg->setLaneInfo(LaneId.c_str());
    populateWSM(msg); //I added this new line of code
    const char* target = msg->getTarget();
    if(strcmp(target, "") == 0) {
        return;
    }
    EV << "MyCarApp::CheckLane::value of target is: " << "[" << target << "] " << "Value of Range::" << msg->getOutOfRange() << std::endl;
    EV << "MyCarApp::CheckLane::value of msgID is:" << msg->getMsgID() << std::endl;
    sendDown(msg);
    EV << "MyCarApp::CheckLane::== " << " " << "sendDown successful " << std::endl;




    /*
    if(distance > 100 && distance <= 800)
    {
        if (traciVehicle->getLaneId() == "Entry_0")
        {
            EV << "MyCarApp::CheckLane:: Vehicle with vehicleId:[ " << VehicleId << "] is on Lane0: "
            << traciVehicle->getLaneId() << std::endl;
            vehicleIsInRange = true;
            msg->setOutOfRange(false);
            std::string laneInfo = "Entry_0";
            msg->setLaneInfo(laneInfo.c_str());
            EV << "MyCarApp::CheckLane::vehicleId:[ " << VehicleId << " ] has ENTERED area of concern "<<std::endl;
        }
        else if (traciVehicle->getLaneId() == "Entry_1")
        {
            EV << "MyCarApp::CheckLane:: Vehicle with vehicleId:[ " << VehicleId << "] is on Lane1: "
            << traciVehicle->getLaneId() << std::endl;
            vehicleIsInRange = true;
            msg->setOutOfRange(false);
            std::string laneInfo = "Entry_1";
            msg->setLaneInfo(laneInfo.c_str());
            EV << "MyCarApp::handlePositionUpdate1::vehicleId:[ " << VehicleId << " ]has ENTERED area of concern "<< std::endl;
        }
        else if (traciVehicle->getLaneId() == "Entry_2")
        {
            EV << "MyCarApp::CheckLane:: Vehicle with vehicleId:[ " << VehicleId << "] is on Lane2: "
            << traciVehicle->getLaneId() << std::endl;
            vehicleIsInRange = true;
            msg->setOutOfRange(false);
            std::string laneInfo = "Entry_2";
            msg->setLaneInfo(laneInfo.c_str());
            EV << "MyCarApp::handlePositionUpdate1::vehicleId:[ " << VehicleId << " ]has ENTERED area of concern "<< std::endl;
        }
    }
    else if(distance > 800)
    {
        vehicleIsInRange = false; //vehicle out of area of concern.
        msg->setOutOfRange(true);
        EV << "MyVeinsAppCar::handlePositionUpdate1:: " << VehicleId <<" has EXITED area of concern" << std::endl;
    }
    */
}




void MyCarApp::printLanes(std::list<std::string> info, std::string msg)
{
    EV <<"MyCarApp::::::printLanes: "  << "is:" << msg << std::endl;
    for( std::string t : info)
    {
        EV << "MyCarApp::MyVeinsAppCar::number of Lanes is: "<< t << std::endl;
    }
}
