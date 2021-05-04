#include "WarningService.h"
#include "warning_msgs/WarningMessage_m.h"
#include "warning_msgs/WarningValidationMessage_m.h"
#include "warning_msgs/OBUBeacon_m.h"
#include "warning_msgs/UpdateTableMessage_m.h"
#include "artery/traci/VehicleController.h"
#include "artery/envmod/LocalEnvironmentModel.h"
#include "AlertTypes.h"
#include "CriticalityTypes.h"
#include "ValidationTypes.h"
#include <vanetza/btp/data_request.hpp>
#include <vanetza/dcc/profile.hpp>
#include <vanetza/geonet/interface.hpp>
#include <iostream>
#include <boost/units/systems/si/prefixes.hpp>
#include "traci/BasicNodeManager.h"
#include "traci/API.h"
#include <artery/application/StoryboardSignal.h>
#include <string.h>
#include <iostream>

using namespace omnetpp;
using namespace vanetza;

// register signal to recieve signal from Storyboard
static const simsignal_t storyboardSignal = cComponent::registerSignal("StoryboardSignal");

Define_Module(WarningService)

void WarningService::initialize() {
    ItsG5BaseService::initialize();
    mVehicleController = &getFacilities().get_mutable<traci::VehicleController>();
    subscribe(storyboardSignal);
    // withReputation = par("withReputation");

    withReputation = false;
    auxTravelTime = registerSignal("auxTravelTime");
    // successSignal = registerSignal("successSignal");
    // delaySignal = registerSignal("delaySignal");
    // distanceSignal = registerSignal("distanceSignal");
    // duplicateSignal = registerSignal("duplicateSignal");
    // insideSignal = registerSignal("insideSignal");
    // outSignal = registerSignal("outSignal");
}

void WarningService::finish() {
}

void WarningService::handleMessage(cMessage* msg) {
    Enter_Method("WarningService::handleMessage");

    if (msg == m_obu_beacon) {
        sendObuBeacon();
    }
}

void WarningService::sendObuBeacon() {
    auto obuBeacon = new OBUBeacon();
    obuBeacon->setIdVei(mVehicleController->getVehicleId().c_str());
    obuBeacon->setLatitude(mVehicleController->getGeoPosition().latitude.value());
    obuBeacon->setLongitude(mVehicleController->getGeoPosition().longitude.value());
    obuBeacon->setTableTime(lastUpdateTable);

    sendPacket2(obuBeacon, mVehicleController->getGeoPosition().latitude.value() * boost::units::degree::degree,
                           mVehicleController->getGeoPosition().longitude.value() * boost::units::degree::degree);

    scheduleAt(simTime() + 5, m_obu_beacon);
}

void WarningService::trigger() {
    Enter_Method("WarningService trigger");

    if (received && initialTime == -1 && mVehicleController->getTraCI()->vehicle.getRoadID(mVehicleController->getVehicleId()) == "-31622#5-AddedOffRampEdge") {
        initialTime = simTime().dbl();
        printf("%s Alterou trajeto %f\n", mVehicleController->getVehicleId().c_str(), initialTime);
    }
    if (received && mVehicleController->getTraCI()->vehicle.getRoadID(mVehicleController->getVehicleId()) == "-31622#10-AddedOnRampEdge" && initialTime > 0) {
        double travelTime = simTime().dbl();
        travelTime = travelTime - initialTime;
        initialTime = -1;
        emit(auxTravelTime, travelTime);
        printf("%s TravelTime %f \n", mVehicleController->getVehicleId().c_str(), travelTime);
    }
    if (withReputation) {
        if ((obuBeacon == false) and (simTime() >= 330) and (mVehicleController->getTraCI()->vehicle.getRoadID(mVehicleController->getVehicleId()) == "-31622#4") and (mVehicleController->getPosition().x.value() < 8600)) {
            m_obu_beacon = new cMessage("OBUBeacon");
            scheduleAt(simTime() + 2, m_obu_beacon);
            obuBeacon = true;
        }
    }

    if (mVehicleController->getVehicleId() == "vehicle_28.566") {

        if (initialTime == -1 && mVehicleController->getTraCI()->vehicle.getRoadID(mVehicleController->getVehicleId()) == "-31622#5-AddedOffRampEdge") {
            initialTime = simTime().dbl();
            printf("============================ \n");
            printf("%s Tempo ate o acidente %f \n", mVehicleController->getVehicleId().c_str(), initialTime);
            printf("%f \n", mVehicleController->getPosition().x.value());
            printf("============================ \n");
        }

        if (mVehicleController->getPosition().x.value() < 3332.97 && initialTime > -1) {
            double travelTime = simTime().dbl();
            travelTime = travelTime - initialTime;
            initialTime = -1;
            printf("============================ \n");
            printf("%s Tempo ate o acidente %f \n", mVehicleController->getVehicleId().c_str(), travelTime);
            printf("%f \n", mVehicleController->getPosition().x.value());
            printf("============================ \n");
        }

        if (initialTime > -1){
            printf("============================ \n");
            printf("%f \n", mVehicleController->getPosition().x.value());
            printf("============================ \n");
        }

    // } && mVehicleController->getTraCI()->vehicle.getRoadID(mVehicleController->getVehicleId()) == "-31622#7") {
    //     double speed = mVehicleController->getTraCI()->vehicle.getSpeed(mVehicleController->getVehicleId());
    //     printf("Speed %f \n", speed);
    }

    // if (initialTime == -1 && mVehicleController->getVehicleId() == "vehicle_27.324") {
    //     std::vector<std::string> route = mVehicleController->getTraCI()->vehicle.getRoute(mVehicleController->getVehicleId());
    //     for (int i = 0; i < route.size() - 1; i++) {
    //         printf("=====================\n");
    //         printf("Route %s \n", route[i].c_str());
    //         printf("=====================\n");
    //     }
    //     initialTime = -2;
    // }

    // -31622#7
    // vehicle_32.5
    // double waitingTime = mVehicleController->getTraCI().edge().getWaitingTime("--31834#1");
    // printf("%f \n", waitingTime);

    // long double dist;
    // long double lat = 49.613308;
    // long double longi = 6.119859;
    // dist = geoDistance(mVehicleController->getGeoPosition().latitude.value(), mVehicleController->getGeoPosition().longitude.value(), lat, longi);

    // if (!inside && dist <= 150){
    //     emit(insideSignal, 1);
    //     inside = true;
    // } else if (inside && dist > 150) {
    //     emit(outSignal, 1);
    //     inside = false;
    // }
}

long double WarningService::toRadians(const long double degree){
    long double oneDeg = (M_PI) / 180;
    return (oneDeg * degree);
}

long double WarningService::geoDistance(long double lat1, long double long1, long double lat2, long double long2){
    lat1 = toRadians(lat1);
    long1 = toRadians(long1);
    lat2 = toRadians(lat2);
    long2 = toRadians(long2);

    long double dlong = long2 - long1;
    long double dlat = lat2 - lat1;

    long double ans = pow(sin(dlat / 2), 2) +
                          cos(lat1) * cos(lat2) *
                          pow(sin(dlong / 2), 2);
    ans = 2 * asin(sqrt(ans));
    long double R = 6371;
    ans = ans * R;
    return ans * 1000;
}

void WarningService::indicate(const vanetza::btp::DataIndication& ind, omnetpp::cPacket* packet) {
    Enter_Method("WarningService indicate");

    if (WarningMessage* warningMessage = dynamic_cast<WarningMessage*>(packet)) {
        if (!received) {
            received = true;
            receiveWarning(warningMessage);
        }
        // } else {
        //     // emit(duplicateSignal, 1);
        // }
    } else if (UpdateTableMessage* tableMessage = dynamic_cast<UpdateTableMessage*>(packet)) {
        // Verificar se a tabela atual esta atualizada
        // Caso não esteja, atualiza sua tabela e seu horario
        if (tableMessage->getTableTime() > lastUpdateTable) {
            receiveTable(tableMessage);
        }
        // if (obuBeacon == true) {
        //     cancelEvent(m_obu_beacon);
        //     scheduleAt(simTime() + 20, m_obu_beacon);
        // }
    }

    delete packet;
}

void WarningService::receiveWarning(const WarningMessage* warningMessage) {
    bool blackList = false;

    if (withReputation) {
        std::string warVehicleId = warningMessage->getVehicleId();
        lastAlertId = warningMessage->getAlertId();
        lastAlertCriticality = warningMessage->getAlertCriticality();

        // Navega na tabela de reputação
        for (struct Reputation vehicleRep: reputationTable) {
            std::string repVehicleId = vehicleRep.idVehicle.c_str();
            if (warVehicleId.compare(repVehicleId) == 0) {
                printf("Veiculo na black list \n");
                if ((vehicleRep.reputation > 0.3) and (warningMessage->getAlertCriticality() == MAX)) {
                    printf("Veiculo considerado suspeiro \n");
                    mVehicleController->getTraCI()->vehicle.rerouteTraveltime(mVehicleController->getVehicleId());
                    mVehicleController->getTraCI()->vehicle.setColor(mVehicleController->getVehicleId(), libsumo::TraCIColor(0, 255, 255, 255));
                }
                blackList = true;
                break;
            } 
        }
    }

    // Se o veículo não esta na blacklist, é pq sua rep > 0.5
    if (blackList == false) {
        printf("Nao esta na blacklist\n");
        mVehicleController->getTraCI()->vehicle.rerouteTraveltime(mVehicleController->getVehicleId());
        // std::vector<std::string> newRoute;

        // newRoute.push_back("-31622#4 ");
        // newRoute.push_back("-31622#4-AddedOffRampEdge ");
        // newRoute.push_back("-31622#5 ");
        // newRoute.push_back("-31622#5-AddedOffRampEdge ");
        // newRoute.push_back("-31760 ");
        // newRoute.push_back("-30706#3-AddedOnRampEdge ");
        // newRoute.push_back("-30706#4 ");
        // newRoute.push_back("-32798-AddedOnRampEdge ");
        // newRoute.push_back("-32798 ");
        // newRoute.push_back("-31876#1 ");
        // newRoute.push_back("-31876#2 ");
        // newRoute.push_back("-31340#0 ");
        // newRoute.push_back("-31340#1 ");
        // newRoute.push_back("-31340#2 ");
        // newRoute.push_back("-31340#3 ");
        // newRoute.push_back("-31340#4 ");
        // newRoute.push_back("--31066#0 ");
        // newRoute.push_back("-32914#1 ");
        // newRoute.push_back("--30348#5 ");
        // newRoute.push_back("--30348#4 ");
        // newRoute.push_back("--30348#3 ");
        // newRoute.push_back("--30348#2 ");
        // newRoute.push_back("--30348#1 ");
        // newRoute.push_back("--30348#0 ");
        // newRoute.push_back("--30892#23 ");
        // newRoute.push_back("--30892#22 ");
        // newRoute.push_back("--30892#21 ");
        // newRoute.push_back("--30892#20 ");
        // newRoute.push_back("--30892#19 ");
        // newRoute.push_back("--30892#18 ");
        // newRoute.push_back("--30892#17 ");
        // newRoute.push_back("-31272#7 ");
        // newRoute.push_back("-31272#8 ");
        // newRoute.push_back("-31690 ");
        // newRoute.push_back("-32858#0 ");
        // newRoute.push_back("-32858#1-AddedOnRampEdge ");
        // newRoute.push_back("-32858#1 ");
        // newRoute.push_back("-32858#1-AddedOffRampEdge ");
        // newRoute.push_back("-32372#0 ");
        // newRoute.push_back("-32372#1 ");
        // newRoute.push_back("-32372#2 ");
        // newRoute.push_back("-31622#10-AddedOnRampEdge ");

        // mVehicleController->getTraCI()->vehicle.setRoute(mVehicleController->getVehicleId(), newRoute);
        mVehicleController->getTraCI()->vehicle.setColor(mVehicleController->getVehicleId(), libsumo::TraCIColor(0, 255, 255, 255));
    }

    // ================================= 
    // Verifica se o veiculo emissor esta na black list
    // Verificar fluxograma
    // =================================    

    // long delayTime = simTime().dbl() - warningMessage->getTimeStamp();
    // artery::Position messagePosition (warningMessage->getX(), warningMessage->getY());
    // artery::Position::value_type totalDistance;
    // totalDistance = distance(mVehicleController->getPosition(), messagePosition);
    // emit(successSignal, 1);
    // emit(delaySignal, delayTime);
    // emit(distanceSignal, (long) totalDistance.value());
}

void WarningService::receiveTable(const UpdateTableMessage* tableMessage) {
    std::string id = mVehicleController->getVehicleId();
    printf("Recebeu atualizacao da tabela -> %s\n", id.c_str());
    lastUpdateTable = tableMessage->getTableTime();
    for (int i = 0; i < 50; i++)
    {
        struct Reputation rep = tableMessage->getReputationTable(i);
        reputationTable.push_back(rep);
    }
}

void WarningService::sendWarningValidationMessage(int messageType) {
    auto validationMessage = new WarningValidationMessage();
    validationMessage->setMessageId("ValidationMessage");
    validationMessage->setAlertId(lastAlertId);
    validationMessage->setAlertCriticality(lastAlertCriticality);
    validationMessage->setMessageType(messageType);
    validationMessage->setTimeStamp(simTime().dbl());

    sendPacket2(validationMessage, 
                    mVehicleController->getGeoPosition().latitude.value() * boost::units::degree::degree,
                    mVehicleController->getGeoPosition().longitude.value() * boost::units::degree::degree);
}

void WarningService::interruptStreet() {
    // std::vector<std::string> disallowedClasses;
    // disallowedClasses.push_back("passenger");
    // mVehicleController->getTraCI().lane().setDisallowed("--31834#1_0", disallowedClasses);
    // mVehicleController->getTraCI().lane().setDisallowed("-31622#7_0", disallowedClasses);
    // mVehicleController->getTraCI().lane().setDisallowed("-31622#7_1", disallowedClasses);
    // mVehicleController->getTraCI().lane().setDisallowed("-31622#7_2", disallowedClasses);

    mVehicleController->getTraCI()->vehicle.moveToXY("brokenVehicle1", "-31622#7", 0, 5032.97, 3527.49, 0, 1);
    mVehicleController->getTraCI()->vehicle.moveToXY("brokenVehicle2", "-31622#7", 1, 5032.97, 3527.49, 0, 1);
    // mVehicleController->getTraCI()->vehicle.moveToXY("brokenVehicle3", "-31622#7", 2, 5032.97, 3527.49, 0, 1);

    mVehicleController->getTraCI()->vehicle.setSpeed("brokenVehicle1", 0.0);
    mVehicleController->getTraCI()->vehicle.setSpeed("brokenVehicle2", 0.0);
    // mVehicleController->getTraCI()->vehicle.setSpeed("brokenVehicle3", 0.0);

    // libsumo::TraCIPositionVector poli;
    // libsumo::TraCIPosition p1;
    // p1.x = 5233;
    // p1.y = 3462;
    // p1.z = 20;
    // libsumo::TraCIPosition p2;
    // p2.x = 5228;
    // p2.y = 3452;
    // p2.z = 20;
    // libsumo::TraCIPosition p3;
    // p3.x = 5236;
    // p3.y = 3450;
    // p3.z = 20;
    // libsumo::TraCIPosition p4;
    // p4.x = 5240;
    // p4.y = 3461;
    // p4.z = 20;
    // poli.push_back(p1);
    // poli.push_back(p2);
    // poli.push_back(p3);
    // poli.push_back(p4);

    // mVehicleController->getTraCI().polygon().add("bloqueio", poli, libsumo::TraCIColor(0, 255, 255, 255), true, "building", 4.00);
}

void WarningService::sendPacket(omnetpp::cPacket* packet) {
    btp::DataRequestB req;
    req.destination_port = host_cast<WarningService::port_type>(getPortNumber());
    req.gn.transport_type = geonet::TransportType::GBC;
    req.gn.traffic_class.tc_id(static_cast<unsigned>(dcc::Profile::DP1));
    req.gn.communication_profile = geonet::CommunicationProfile::ITS_G5;

    geonet::Area area;
    area.shape = geonet::Circle();

    using units::si::meters;
    geonet::Circle destination_shape;
    destination_shape.r = 500 * meters; // A DEFINIR
    area.shape = destination_shape;

    units::GeoAngle latitude {49.576090 * boost::units::degree::degree};
    units::GeoAngle longitude {6.145842 * boost::units::degree::degree};


    EV_INFO << "==================" << endl;
    EV_INFO << latitude.value() << endl;
    EV_INFO << longitude.value() << endl;
    EV_INFO << "==================" << endl;

    area.position.latitude = latitude;
    area.position.longitude = longitude;
    req.gn.destination = area;
    req.gn.maximum_hop_limit = 100; // A DEFINIR

    request(req, packet);
}

void WarningService::sendPacket2(omnetpp::cPacket* packet, units::GeoAngle latitude, units::GeoAngle longitude) {
    btp::DataRequestB req;
    req.destination_port = host_cast<WarningService::port_type>(getPortNumber());
    req.gn.transport_type = geonet::TransportType::GBC;
    req.gn.traffic_class.tc_id(static_cast<unsigned>(dcc::Profile::DP1));
    req.gn.communication_profile = geonet::CommunicationProfile::ITS_G5;

    geonet::Area area;
    area.shape = geonet::Circle();

    using units::si::meters;
    geonet::Circle destination_shape;
    destination_shape.r = 500 * meters; // A DEFINIR
    area.shape = destination_shape;

    area.position.latitude = latitude;
    area.position.longitude = longitude;
    req.gn.destination = area;
    req.gn.maximum_hop_limit = 100; // A DEFINIR

    request(req, packet);
}

void WarningService::receiveSignal(omnetpp::cComponent*, omnetpp::simsignal_t sig, omnetpp::cObject* sigobj, omnetpp::cObject*) {
    Enter_Method("WarningService receiveSignal");
    if (sig == storyboardSignal) {
        auto storysig = dynamic_cast<artery::StoryboardSignal*>(sigobj);
        if (storysig && storysig->getCause() == "stop") {
            printf("Recebeu STOP \n");
            interruptStreet();
        } else if (storysig && storysig->getCause() == "scenario_1") {
            printf("Recebeu Scenario 1 \n");
            const std::string id = mVehicleController->getVehicleId();
            auto& vehicle_api = mVehicleController->getTraCI()->vehicle;

            auto packet = new WarningMessage();
            packet->setMessageId("message1");
            packet->setVehicleId(id.c_str());
            packet->setAlertId(WARNING);
            packet->setAlertCriticality(MAX);
            packet->setTimeStamp(simTime().dbl());
            packet->setX(mVehicleController->getPosition().x.value());
            packet->setY(mVehicleController->getPosition().y.value());

            sendPacket(packet);
        } else if (storysig && storysig->getCause() == "propagate") {
            const std::string id = mVehicleController->getVehicleId();
            auto& vehicle_api = mVehicleController->getTraCI()->vehicle;

            auto packet = new WarningMessage();
            packet->setMessageId("message1");
            packet->setVehicleId(id.c_str());
            packet->setAlertId(WARNING);
            packet->setAlertCriticality(MAX);
            packet->setTimeStamp(simTime().dbl());
            packet->setX(mVehicleController->getPosition().x.value());
            packet->setY(mVehicleController->getPosition().y.value());

            sendPacket(packet);
        } else if (storysig && storysig->getCause() == "do_not_propagate") {
            interruptStreet();
        } else if (storysig && storysig->getCause() == "fake_message") {
            const std::string id = mVehicleController->getVehicleId();
            auto& vehicle_api = mVehicleController->getTraCI()->vehicle;

            auto packet = new WarningMessage();
            // packet->setMessageId(1);
            // packet->setVehicleId(id.c_str());
            // packet->setTimeStamp(simTime().dbl());
            // packet->setEdgeName(vehicle_api.getRoadID(id).c_str());
            // packet->setLaneIndex(vehicle_api.getLaneIndex(id));
            // packet->setType(1);
            // packet->setX(mVehicleController->getPosition().x.value());
            // packet->setY(mVehicleController->getPosition().y.value());

            sendPacket(packet);
        } else if (storysig && storysig->getCause() == "inside") {
            interruptStreet();

            EV_INFO << "========================" << endl;
            EV_INFO << "Recebeu Story -> inside " << endl;

            const std::string id = mVehicleController->getVehicleId();
            auto& vehicle_api = mVehicleController->getTraCI()->vehicle;

            auto packet = new WarningMessage();
            // packet->setMessageId(1);
            // packet->setVehicleId(id.c_str());
            // packet->setTimeStamp(simTime().dbl());
            // packet->setEdgeName(vehicle_api.getRoadID(id).c_str());
            // packet->setLaneIndex(vehicle_api.getLaneIndex(id));
            // packet->setType(1);
            // packet->setX(mVehicleController->getPosition().x.value());
            // packet->setY(mVehicleController->getPosition().y.value());

            EV_INFO << mVehicleController->getGeoPosition().latitude.value() << endl;
            EV_INFO << mVehicleController->getGeoPosition().longitude.value() << endl;
            EV_INFO << "========================" << endl;


            sendPacket2(packet, 
                        mVehicleController->getGeoPosition().latitude.value() * boost::units::degree::degree,
                        mVehicleController->getGeoPosition().longitude.value() * boost::units::degree::degree);
        }
    }
}
