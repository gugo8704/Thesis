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
#include "veins/modules/mobility/traci/TraCIScenarioManagerLaunchd.h"
#include "veins/modules/application/ieee80211p/DemoBaseApplLayer.h"
#include "veins/modules/messages/InitialMessage_m.h"
#include "veins/modules/messages/RSU_m.h"
#include "string"
#include "set"
#include <sstream>
namespace veins {

//Small RSU Demo using 11p
class MyRSUApp0 : public DemoBaseApplLayer {
protected:
    void initialize(int stage) override;

    void onBSM(DemoSafetyMessage* bsm) override;
    void onWSM(BaseFrame1609_4* wsm) override;
    //void handlePositionUpdate(cObject* obj) override;
    void handleSelfMsg(cMessage* msg) override;
    int CarId;
    std::string rsuId;
    std::string commaSeparatedString;

    void DetectLane(std::string laneInfoStr, int currentCarId);
    std::set<std::string> setOfVehicles_Entry_0;
    std::set<std::string> setOfVehicles_Entry_1;
    std::set<std::string> setOfVehicles_Entry_2;
    std::set<std::string> setOfVehicles_Exit_0;
    std::set<std::string> setOfVehicles_Exit_1;
    std::set<std::string> setOfVehicles_Exit_2;

    static std::set<std::string>* laneSet;


};

} // namespace veins
