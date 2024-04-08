Project Name: Traffic Flow regulation using Vehicular Communication

Overview
The high inflow of traffic due to the increased number of cars, trucks, buses, and other vehicles in recent years has fueled problems like traffic congestion, driving risks, and
accident possibilities during adverse weather conditions. The pressing need for smart traffic flow optimization and enhancing road safety has become paramount which can be
effectively addressed by adopting cutting-edge technologies like Intelligent Transportation Systems (ITS) based Vehicle-to-Infrastructure (V2I) solutions. To enhance road traffic
safety, it is essential to address challenges related to estimating traffic density and regulation of traffic speed. Integrating V2I based communication between Infrastructure
Road Side Units (RSUs) and the vehicles, can effectively address the traffic optimization concerns through the dissemination of advisory speed limits.

This project introduces a novel approach about speed limit estimation for each lane on a multi-lane highway. This is achieved by implementing a lane detection algorithm,
enabling vehicles to continuously share real-time traffic data about the specific lane they are currently driving with RSUs. The shared data is then used by RSUs to calculate
the lane-specific vehicular densities. Subsequently, RSU determines the speed limit of the lanes and broadcast the advisory speed limit to the vehicles depending upon various
scenarios.

Technologies Used
C++
OMNeT++
Veins (Vehicle Network Simulation)

Installation Instructions
Install OMNeT++: [https://doc.omnetpp.org/omnetpp/InstallGuide.pdf].
Install Veins: [https://veins.car2x.org/download/].
Clone Repository: Clone this repository to your local machine using git clone.
Build Project: Follow the build instructions provided as per the OMNeT++ installation link.
