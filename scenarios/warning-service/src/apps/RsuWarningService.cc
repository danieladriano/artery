#include "RsuWarningService.h"
#include "warning_msgs/OBUBeacon_m.h"
#include "warning_msgs/UpdateTableMessage_m.h"
#include "warning_msgs/WarningValidationMessage_m.h"
#include "ValidationTypes.h"
#include "artery/envmod/LocalEnvironmentModel.h"
#include <vanetza/btp/data_request.hpp>
#include <vanetza/dcc/profile.hpp>
#include <vanetza/geonet/interface.hpp>
#include <iostream>
#include <boost/units/systems/si/prefixes.hpp>
#include "traci/BasicNodeManager.h"
#include <artery/application/StoryboardSignal.h>
#include <string.h>
#include <iostream>
#include "artery/utility/Geometry.h"
    
using namespace omnetpp;
using namespace vanetza;


Define_Module(RsuWarningService)

void RsuWarningService::initialize(){
    ItsG5BaseService::initialize();
    // mIdentity = &getFacilities().get_const<Identity>();
    // mGeoPosition = &getFacilities().get_const<GeoPosition>();

    // Somente para testar o recebimento do OBUBeacon e envio da possivel tabela
    lastUpdateTable = 10;

    // withReputation = par("withReputation");
    withReputation = false;

    if (withReputation) {
        m_update_blockchain = new cMessage("UpdateBlockChain");
        scheduleAt(simTime() + 20, m_update_blockchain);
    }

    m_resend_warning_message = new cMessage("ResendWarningMessage");

    wmSignal = registerSignal("warning");
}

void RsuWarningService::trigger() {

}

void RsuWarningService::indicate(const vanetza::btp::DataIndication& ind, omnetpp::cPacket* packet) {
    Enter_Method("RsuWarningService indicate");

    if (OBUBeacon* obuBeacon = dynamic_cast<OBUBeacon*>(packet)) {
        receiveObuBeacon(obuBeacon);
    } else if (WarningValidationMessage* validation = dynamic_cast<WarningValidationMessage*>(packet)) {
        receiveValidationMessage(validation);
    } else if (WarningMessage* warningMessage = dynamic_cast<WarningMessage*>(packet)) {
        if (!received) {
            received = true;
            receiveWarning(warningMessage);
            emit(wmSignal, 1);
        }
    }

    // RSU dentro do roi Recebe mensagem de alerta
    // Armazena ela
    // Fica repetindo ela de X em X segundos

    // RSU no caminho e recebe mensagem de alerta
    // Monitorar se o veiculo relay node retransmitiu a mensagem
    // Caso negativo, diminui rep do veiculo relay node e atualiza tabela das demais RSUs

    delete packet;
}

void RsuWarningService::handleMessage(cMessage* msg) {
    Enter_Method("RsuWarningService::handleMessage");

    if (msg == m_update_blockchain) {
        calcReputation();
    } else if (msg == m_resend_warning_message) {
        resendWarningMessage();
    }
}

void RsuWarningService::sendUpdateTableMessage() {
    auto tableMessage = new UpdateTableMessage();
    tableMessage->setIdRsu("RSU1");
    tableMessage->setReputationTableArraySize(50);
    for (int i = 0; i < 50; i++)
    {
        struct Reputation rep;
        rep.idVehicle = "brokenVehicle1";
        rep.reputation = 0.4;
        tableMessage->setReputationTable(i, rep);        
    }
    tableMessage->setTableTime(lastUpdateTable);

    sendPacket(tableMessage);
}

void RsuWarningService::receiveObuBeacon(const OBUBeacon* obuBeacon) {
    // Vai receber Beacon
    // Verificar se o tempo da tabela de rep do veiculo atual
    // Caso não seja, faz um GeoBroadcast passando idVeic + TabelaAtualizada + HorarioTabela

    if (obuBeacon->getTableTime() < lastUpdateTable) {
        // printf("Recebeu OBU Beacon\n");
        sendUpdateTableMessage();
    }
}

void RsuWarningService::receiveValidationMessage(const WarningValidationMessage* validation) {
    if (validation->getMessageType() == ACK) {
        acks++;
    } else {
        nacks++;
    }
}

void RsuWarningService::receiveWarning(const WarningMessage* warningMesg) {
    auto warningMessage = new WarningMessage();
    warningMessage->setMessageId(warningMesg->getMessageId());
    warningMessage->setVehicleId(warningMesg->getVehicleId());
    warningMessage->setAlertId(warningMesg->getAlertId());
    warningMessage->setAlertCriticality(warningMesg->getAlertCriticality());
    warningMessage->setTimeStamp(warningMesg->getTimeStamp());
    warningMessage->setX(warningMesg->getX());
    warningMessage->setY(warningMesg->getY());

    scheduleAt(simTime() + 15, m_resend_warning_message);
}

void RsuWarningService::resendWarningMessage(){
    if (received) {
        auto packet = new WarningMessage();
        packet->setMessageId(warningMessage.getMessageId());
        packet->setVehicleId(warningMessage.getVehicleId());
        packet->setAlertId(warningMessage.getAlertId());
        packet->setAlertCriticality(warningMessage.getAlertCriticality());
        packet->setTimeStamp(warningMessage.getTimeStamp());
        packet->setX(warningMessage.getX());
        packet->setY(warningMessage.getY());
        
        sendPacket(packet);
        scheduleAt(simTime() + 15, m_resend_warning_message);
    }
}

void RsuWarningService::calcReputation() {
    float rsuFeed = acks / (acks + nacks);
    if (rsuFeed >= 0.55) {
        // Mensagem considerada verdadeira
        // repNew = repAct + repAct * rsuFeed - (repAct)**2 * rsuFeed
        // repAct = reputacao atual
        float repAct = 0;
        float repNew = repAct + (repAct * rsuFeed) - ((repAct * repAct) * rsuFeed);
    } else if (rsuFeed <= 0.45) {
        // Mensagem considerada falsa
        // repNew = (repAct + repAct * rsuFeed) / 2
        float repAct = 0;
        float repNew = (repAct + repAct * rsuFeed) / 2;
    }
    scheduleAt(simTime() + 20, m_update_blockchain);
}

void RsuWarningService::finish() {
    
}

void RsuWarningService::sendPacket(omnetpp::cPacket* packet) {
    // O envio do pacote com a tabela atualizada será via qual protocolo?
    // GeoBroadcast? GeoUnicast?

    btp::DataRequestB req;
    req.destination_port = host_cast<RsuWarningService::port_type>(getPortNumber());
    req.gn.transport_type = geonet::TransportType::SHB;
    req.gn.traffic_class.tc_id(static_cast<unsigned>(dcc::Profile::DP1));
    req.gn.communication_profile = geonet::CommunicationProfile::ITS_G5;

    // geonet::Area area;
    // area.shape = geonet::Circle();

    // using units::si::meters;
    // geonet::Circle destination_shape;
    // destination_shape.r = 300 * meters; // A DEFINIR
    // area.shape = destination_shape;

    // units::GeoAngle latitude {49.575947 * boost::units::degree::degree};
    // units::GeoAngle longitude {6.145914 * boost::units::degree::degree};

    // units::GeoAngle latitude = mVehi

    // area.position.latitude = latitude;
    // area.position.longitude = longitude;
    // req.gn.destination = area;
    // req.gn.maximum_hop_limit = 100; // A DEFINIR

    request(req, packet);
}