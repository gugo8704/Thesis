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

#include "MyRSUApp0.h"
#include "veins/modules/application/traci/TraCIDemo11pMessage_m.h"

using namespace veins;

Define_Module(veins::MyRSUApp0);
//std::set<std::string> MyRSUApp0::setOfVehicles;
std::set<std::string>* MyRSUApp0::laneSet = nullptr;
void MyRSUApp0::initialize(int stage)
{
   DemoBaseApplLayer::initialize(stage);
   EV << "MyRSUApp0::::initialize: " << std::endl;
   if (stage == 0) {
   EV << "MyRSUApp0::initialize: myID=" << this->myId <<", getId="<<getId()
   <<", appName="<<par("appName").stringValue()
   <<", RSUId="<<par("RSUId").stringValue()
   << std::endl;

  //28.05.2023
  cMessage* cmsg = new cMessage();
  scheduleAt(simTime() + 10, cmsg);
  }
}

void MyRSUApp0::onBSM(DemoSafetyMessage* bsm)
{
    EV <<"MyVeinsAppRSU0::onBSM::send message RSU id:" << "  Receive successfully !!!!!!!!!!!" << std::endl;
    EV <<"bc->getRSUId()"<<std::endl;
}

void MyRSUApp0::onWSM(BaseFrame1609_4* frame)
{
    EV << "MyRSUApp0::onWSM::" << std::endl;
    auto currentCarId = -1;
    if(InitialMessage* msgFromCar = dynamic_cast<InitialMessage*>(frame))
    {
        EV <<"MyRSUApp0::onWSM::Inside if::msgFromCar typecast is successful" << endl;
        const char* target = msgFromCar->getTarget();
        EV << "MyRSUApp0::onWSM::inside strcmp:: value of target is:" << target << std::endl;
        EV << "MyRSUApp0::onWSM::value of msgID is:" << msgFromCar->getMsgID() << std::endl;

        findHost()->getDisplayString().updateWith("r=16,green");
        EV << "MyRSUApp0::onWSM::RSUs turn green:" << getId() << std::endl;
        if (strcmp(msgFromCar->getTarget(), "MyRSUApp0")==0)
        {

            EV <<"MyRSUApp0::onWSM::Inside if statement::msgFromCar dynamic_cast" << "MyRSUApp0" << std::endl;
            if(!msgFromCar->getOutOfRange())
            {
                EV <<"MyRSUApp0::onWSM::Inside if::if msg not outofrange" << "MyRSUApp0" << std::endl;
                currentCarId = std::stoi(msgFromCar->getName());
                const char* laneInfo = msgFromCar->getLaneInfo();
                std::string laneInfoStr(laneInfo); // Convert to a string
                //use string stream to split the 'laneInfo'
                if(laneInfoStr == "Entry_0"|| laneInfoStr == "Entry_1" || laneInfoStr == "Entry_2")
                {
                    DetectLane(laneInfoStr, currentCarId);
                    EV <<"MyRSUApp0::onWSM::Inside if statement::check call lnaeInfostr" << std::endl;
                }
                else if(laneInfoStr == "Exit_0" || laneInfoStr == "Exit_1" || laneInfoStr == "Exit_2")
                {
                    DetectLane(laneInfoStr, currentCarId);
                }
            }   //'getOutOfRange' condition:66 ends.
            }  //if(strcmp == this msg for me):line 62 ends.

            else if(strcmp(msgFromCar->getTarget(), "MyRSUApp0.erase")==0)//erase part, when vehicles have crossed area of concern
            {
                EV <<"MyRSUApp0::onWSM::Inside else if::msgFromCar erase" << endl;
                currentCarId = std::stoi(msgFromCar->getName());
                const char* laneInfo = msgFromCar->getLaneInfo();
                std::string laneInfoStr(laneInfo); // Convert to a string
                if(laneInfoStr == "Exit_0"|| laneInfoStr == "Exit_1" || laneInfoStr == "Exit_2")
                {
                    DetectLane(laneInfoStr, currentCarId);
                }
            }
        /*
         else
         assert(false);//helps debugging
         */

    }

    else if(RSU* msgFromRSU1 = dynamic_cast<RSU*>(frame))
    {
        EV <<"MyRSUApp0::onWSM::typecast is successful:  "<< endl;
        if (strcmp(msgFromRSU1->getRsutarget(), "MyRSUApp0")==0)
        {
            currentCarId = std::stoi(msgFromRSU1->getName());
            const char* laneInfo = msgFromRSU1->getRsulaneInfo();
            std::string laneInfoStr(laneInfo);
            EV <<"MyRSUApp0::onWSM::currentCarId:: " << currentCarId << endl;
            if(!msgFromRSU1->getOutOfRangersu())
            {
                if(!laneInfoStr.empty())
                {
                    EV <<"MyRSUApp0::onWSM::Inside else if statement: RSU1 -> RSU0: "<< std::endl;
                    if(laneInfoStr == "Exit_0" || laneInfoStr == "Exit_1" || laneInfoStr == "Exit_2")
                    {
                        DetectLane(laneInfoStr, currentCarId);
                    }
                }
            }
        }//aab yha pe code aayega
        else if (strcmp(msgFromRSU1->getRsutarget(), "MyRSUApp0.erase")==0)
        {
            EV <<"MyRSUApp0::onWSM::Inside erase else if:L-R:" << endl;
            currentCarId = std::stoi(msgFromRSU1->getName());
            const char* laneInfo = msgFromRSU1->getRsulaneInfo();
            std::string laneInfoStr(laneInfo);
            EV <<"MyRSUApp0::onWSM::currentCarId:: " << currentCarId << endl;
            if(laneInfoStr == "Entry_0"|| laneInfoStr == "Entry_1" || laneInfoStr == "Entry_2")
            {
                DetectLane(laneInfoStr, currentCarId);
            }
        }
    }
}

void MyRSUApp0::DetectLane(std::string laneInfoStr, int currentCarId)
{
    int newCarSpeed;
    EV << "MyRSUApp0::DetectLane::" << std::endl;
    EV << "MyRSUApp0::DetectLane::LaneInfoStr= " << laneInfoStr  << " , CurrentCarId= " << currentCarId<< std::endl;
    RSU* rsuMsg = new RSU(); //new 'RSU' msg

    // Determine the set based on the laneInfoStr
        if (laneInfoStr == "Entry_0")
        {
            laneSet = &setOfVehicles_Entry_0;
        }
        else if (laneInfoStr == "Entry_1")
        {
            laneSet = &setOfVehicles_Entry_1;
        }
        else if (laneInfoStr == "Entry_2")
        {
            laneSet = &setOfVehicles_Entry_2;
        }
        else if (laneInfoStr == "Exit_0")
        {
            laneSet = &setOfVehicles_Exit_0;

            // Get the size of setOfVehicles_Entry_0
            std::size_t setSize = setOfVehicles_Exit_0.size();

            // Print the size
            EV << "MyRSUApp0::DetectLane::Size of setOfVehicles_Exit_0: " << setSize <<
            " , CurrentCarId= " << currentCarId<<std::endl;
        }
        else if (laneInfoStr == "Exit_1")
        {
            laneSet = &setOfVehicles_Exit_1;
        }
        else if (laneInfoStr == "Exit_2")
        {
            laneSet = &setOfVehicles_Exit_2;
        }

        if (laneSet != nullptr)
        {
            bool VehicleAlreadyIntoRSUSet = (laneSet->find(std::to_string(currentCarId)) != laneSet->end());
            if (!VehicleAlreadyIntoRSUSet)
            {
                laneSet->insert(std::to_string(currentCarId));
                VehicleAlreadyIntoRSUSet = true;
                EV << "MyRSUApp0::onWSM::adding node: " << currentCarId <<  " into RSU set" << std::endl;
            }
            EV << "MyRSUApp0::onWSM::set of laneSet is: "<< simTime() << " " << laneInfoStr << " " << "[" <<laneSet->size() << "]" << " && " << currentCarId << std::endl;
            for (const auto& car : *laneSet)
            {
                if(!commaSeparatedString.empty())
                {
                    commaSeparatedString += ",";
                }
                commaSeparatedString += car;//end of 'for loop'
            }   // 'for loop:135' ends here
            EV <<"MyRSUApp0::onWSM::size of laneSet in the simulation: "<< laneSet->size() << " && " << currentCarId << std::endl;

            if (laneSet->size()>=5)//18.05.2023
            {
                newCarSpeed = 25;
                EV << "MyRSUApp0::onWSM::msg sending down2 "<< std::endl;
            }
            else if(laneSet->size()>=4)
            {
                newCarSpeed = 28;
                EV << "MyRSUApp0::onWSM::msg sending down3 "<< std::endl;

            }
            else if(laneSet->size() >=2)
            {
                newCarSpeed = 30;
                EV << "MyRSUApp0::onWSM::msg sending down4 "<< std::endl;
            }
            else if(laneSet->size() >=1)
            {
                newCarSpeed = 32;
                EV << "MyRSUApp0::onWSM::msg sending down5 "<< std::endl;
            }
            else
            {
                newCarSpeed = 36.11;
                EV << "MyRSUApp0::onWSM::msg sending down6 "<< std::endl;

            }
            EV << "MyRSUApp0::onWSM:: sending NewSpeed= " <<newCarSpeed<< "  to [" <<commaSeparatedString<< "]"<<std::endl;
            // Send new speed to list of cars in the area of concern
            rsuMsg->setList(commaSeparatedString.c_str()); //setting the string field
            rsuMsg->setSpeed(newCarSpeed);
            populateWSM(rsuMsg);
            rsuMsg->addByteLength((laneSet->size())*4);
            sendDown(rsuMsg);
            //int Byte = rsuMsg->getByteLength();
            //int numberofBits = rsuMsg->getBitLength();
            EV << "MyRSUApp0::onWSM:: sending NewSpeed= " <<newCarSpeed<< "  to [" <<commaSeparatedString<< "]"<<std::endl;
            EV << "MyRSUApp0::onWSM:: senddown successful:" << std::endl;
            EV << "MyRSUApp0::DetectLane::number of bits= [" <<rsuMsg->getBitLength() << "]" <<std::endl;
            EV << "MyRSUApp0::DetectLane::number of bytes= [" <<rsuMsg->getByteLength() << "]" <<std::endl;
        } // condition (laneSet != nullptr):125 end.

    else
    {
        //laneSet->erase(std::to_string(currentCarId));
        EV << "MyRSUApp0::onWSM::reduced size when vehicle density reduced:" << laneSet->size() << " && " << currentCarId << std::endl;
        //rsuMsg->setSpeed(-1);
        //sendDown(rsuMsg);
        //laneSet->erase(std::to_string(currentCarId));
    //EV << "MyVeinsAppRSU0::onWSM::reduced size when vehicle density reduced:" << laneSet->size() << " && " << currentCarId << std::endl;
    }
}


void MyRSUApp0::handleSelfMsg(cMessage* msg)
{
    DemoBaseApplLayer::handleSelfMsg(msg);
    EV << "MyRSUApp0::handleSelfMsg:: " << std::endl;
    //EV << "Received object type: " << typeid(*frame).name() << endl;
    //EV << "Received object size: " << sizeof(*frame) << " bytes" << endl;
}
