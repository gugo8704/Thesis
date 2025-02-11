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

#pragma once

#include "veins/veins.h"
#include <list>
#include <string>
#include "veins/modules/application/ieee80211p/DemoBaseApplLayer.h"
#include "veins/modules/messages/InitialMessage_m.h"
#include "veins/modules/messages/RSU_m.h"

using namespace omnetpp;

namespace veins {

/**
 * @brief
 * This is a stub for a typical Veins application layer.
 * Most common functions are overloaded.
 * See MyCarApp.cc for hints
 *
 * @author David Eckhoff
 *
 */

class MyCarApp : public DemoBaseApplLayer {
public:
    void initialize(int stage) override;
    void finish() override;

protected:
    void onBSM(DemoSafetyMessage* bsm) override;
    void onWSM(BaseFrame1609_4* wsm) override;
    //void onWSA(DemoServiceAdvertisment* wsa) override;

    void handleSelfMsg(cMessage* msg) override;
    void handlePositionUpdate(cObject* obj) override;
    cMessage* newmsg;
    std::string VehicleTypeId;
    static int totalCount; //declaration
    int VehicleId;
    bool VehiclesAlreadyCounted;
    Coord cJunctioncoord; //parameter declaration to calculate junction Coords
    void printLanes(std::list<std::string>, std::string);
    void CheckLane(TraCICommandInterface::Vehicle* traciVehicle, std::string LaneId, int VehicleId, double distance, bool IsEntryLane);
    std::string currentLaneId;
    //std::string VehicleId;
    static std::set<std::string> setOfCars;

    //int numInitStages() const override
        //{
        //    DemoBaseApplLayer::numInitStages() + 1;
        //}

};
int MyCarApp::totalCount =0;
} // namespace veins
