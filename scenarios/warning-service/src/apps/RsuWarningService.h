#ifndef RSUWARNINGSERVICE_H_
#define RSUWARNINGSERVICE_H_

#include "artery/application/ItsG5Service.h"
#include "warning_msgs/OBUBeacon_m.h"
#include "warning_msgs/WarningValidationMessage_m.h"
#include "warning_msgs/WarningMessage_m.h"


class RsuWarningService: public artery::ItsG5Service {
    protected:
        void initialize() override;
        void trigger() override;
        void indicate(const vanetza::btp::DataIndication&, omnetpp::cPacket*) override;
        void finish() override;
        void handleMessage(omnetpp::cMessage*) override;
    private:
        omnetpp::cMessage* m_update_blockchain;
        omnetpp::cMessage* m_resend_warning_message;
        int acks = 1;
        int nacks = 1;
        bool received = false;
        WarningMessage warningMessage;
        bool withReputation = true;

        // const Identity* mIdentity = nullptr;
        // GeoPositon rsuGeoPosition;


        double lastUpdateTable = -1;
        void receiveObuBeacon(const OBUBeacon*);
        void receiveValidationMessage(const WarningValidationMessage*);
        void receiveWarning(const WarningMessage*);
        void sendPacket(omnetpp::cPacket*);
        void sendRSUBeacon();
        void sendUpdateTableMessage();
        void calcReputation();
        void resendWarningMessage();
};

#endif /* RSUWARNINGSERVICE_H_ */