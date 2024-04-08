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

#include "MyRSUApp1.h"
#include "veins/modules/application/ieee80211p/DemoBaseApplLayer.h"
#include "veins/modules/application/traci/TraCIDemo11pMessage_m.h"

using namespace veins;

Define_Module(veins::MyRSUApp1);
//std::set<std::string> MyRSUApp1::setOfVehicles;
std::set<std::string>* MyRSUApp1::laneSet = nullptr;
void MyRSUApp1::initialize(int stage)
{
    EV << "MyRSUApp1::::initialize: " << std::endl;
    DemoBaseApplLayer::initialize(stage);
    if (stage == 0) {
        EV << "MyRSUApp1::initialize: myID=" << this->myId <<", getId="<<getId()
        <<", appName="<<par("appName").stringValue()
        <<", RSUId="<<par("RSUId").stringValue() << std::endl;
        //28.05.2023
        cMessage* cmsg = new cMessage();
        scheduleAt(simTime() + 10, cmsg);
    }
}

void MyRSUApp1::onBSM(DemoSafetyMessage* bsm)
{
    EV <<"MyRSUApp1::onBSM::send message RSU id:" << "  Receive successfully !!!!!!!!!!!" << std::endl;
    EV <<"bc->getRSUId()"<<std::endl;
}

void MyRSUApp1::onWSM(BaseFrame1609_4* frame)
{
    EV << "MyRSUApp1::onWSM::" << std::endl;
    auto receivedVehicleId = -1;
    if(InitialMessage* msgreceived = dynamic_cast<InitialMessage*>(frame))
    {
        EV << "MyRSUApp1::onWSM::inside if statement:  " << std::endl;
        //if msg for RSU1 then only process, otherwise (if 'not equal to') discard
        if (strcmp(msgreceived->getTarget(), "MyRSUApp1")==0)
        {
            findHost()->getDisplayString().updateWith("r=16,green");
            receivedVehicleId = std::stoi(msgreceived->getName());
            RSU* msgforRSU0 = new RSU(std::to_string(receivedVehicleId).c_str());   //new msg
            const char* laneInfo = msgreceived->getLaneInfo();
            std::string laneInfoStr(laneInfo); // Convert to a string
            if(!msgreceived->getOutOfRange())
            {
                msgforRSU0->setRsutarget("MyRSUApp0");  ////forward VehicleID to RSU0
                msgforRSU0->setRsulaneInfo(laneInfoStr.c_str());
                sendDown(msgforRSU0);
                EV << "MyRSUApp1::onWSM::sendDown():Sending message of type:" << typeid(*msgforRSU0).name() << std::endl;
            }
        }
        else if(strcmp(msgreceived->getTarget(), "MyRSUApp1.erase")==0)
        {
            receivedVehicleId = std::stoi(msgreceived->getName());
            EV << "MyRSUApp1::onWSM::receivedVehicleId for erase Id:" << receivedVehicleId << endl;
            RSU* msgforRSU0 = new RSU(std::to_string(receivedVehicleId).c_str());   //new msg
            msgforRSU0->setRsutarget("MyRSUApp0.erase");
            sendDown(msgforRSU0);
            EV << "MyRSUApp1::onWSM::sendDown():Sending down erase Id:" << typeid(*msgforRSU0).name() << std::endl;
        }
    }
}

void MyRSUApp1::handleSelfMsg(cMessage* msg)
{
    DemoBaseApplLayer::handleSelfMsg(msg);
    EV << "MyRSUApp1::handleSelfMsg:: " << std::endl;
}
