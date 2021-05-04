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
        bool obuBeacon = false;
        double initialTime = -1;
        double lastUpdateTable = -1;
        std::vector<struct Reputation> reputationTable;
        int lastAlertId = -1;
        int lastAlertCriticality = -1;
        bool withReputation = true;

        traci::VehicleController* mVehicleController = nullptr;
        omnetpp::simsignal_t auxTravelTime;
        // omnetpp::simsignal_t successSignal;
        // omnetpp::simsignal_t delaySignal;
        // omnetpp::simsignal_t distanceSignal;
        // omnetpp::simsignal_t duplicateSignal;
        // omnetpp::simsignal_t insideSignal;
        // omnetpp::simsignal_t outSignal;

        void interruptStreet();
        void sendPacket(omnetpp::cPacket*);
        void sendPacket2(omnetpp::cPacket*, vanetza::units::GeoAngle, vanetza::units::GeoAngle);
        long double geoDistance(long double, long double, long double, long double);
        long double toRadians(const long double);
        void sendObuBeacon();
        void sendWarningValidationMessage(int);
        void receiveTable(const UpdateTableMessage*);
        void receiveWarning(const WarningMessage*);
};

#endif /* WARNINGSERVICE_H_ */