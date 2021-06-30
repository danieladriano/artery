#ifndef WARNINGSERVICE_H_
#define WARNINGSERVICE_H_

#include "artery/application/ItsG5Service.h"
#include "warning_msgs/WarningMessage_m.h"
#include "warning_msgs/UpdateTableMessage_m.h"
#include <fstream>


namespace traci { class VehicleController; }

class WarningService: public artery::ItsG5Service {
    protected:
        omnetpp::cMessage* m_obu_beacon;

        void initialize() override;
        void trigger() override;
        void indicate(const vanetza::btp::DataIndication&, omnetpp::cPacket*) override;
        void receiveSignal(omnetpp::cComponent*, omnetpp::simsignal_t, omnetpp::cObject*, omnetpp::cObject*) override;
        void finish() override;
        void handleMessage(omnetpp::cMessage*) override;
    private:
        bool received = false;
        bool inside = false;

        double lastUpdateTable = -1;
        double initialTravelTime = -1;
        bool changeRoute = false;

        std::vector<struct Reputation> reputationTable;
        int lastAlertId = -1;
        int lastAlertCriticality = -1;
        bool withReputation = true;

        traci::VehicleController* mVehicleController = nullptr;
        omnetpp::simsignal_t mainTravelTimeSignal;
        omnetpp::simsignal_t auxTravelTimeSignal;
        omnetpp::simsignal_t changeRouteSignal;
        omnetpp::simsignal_t receivedWMSignal;

        omnetpp::simsignal_t totalVehiclesSignal;
        omnetpp::simsignal_t totalVehiclesHighwaySignal;

        void interruptStreet();
        void sendPacket(omnetpp::cPacket*);
        void sendPacket2(omnetpp::cPacket*, vanetza::units::GeoAngle, vanetza::units::GeoAngle);
        long double geoDistance(long double, long double, long double, long double);
        long double toRadians(const long double);
        void sendObuBeacon();
        void sendWarningValidationMessage(int);
        void receiveTable(const UpdateTableMessage*);
        void receiveWarning(const WarningMessage*);
        void writeLog(std::string);
        bool isVehicleInValidEdge();
        void changeVehicleRoute();
};

#endif /* WARNINGSERVICE_H_ */