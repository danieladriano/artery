add_artery_feature(warning_service src/apps/WarningService.cc src/apps/RsuWarningService.cc)
generate_opp_message(src/msgs/WarningMessage.msg TARGET warning_service DIRECTORY warning_msgs)
generate_opp_message(src/msgs/UpdateTableMessage.msg TARGET warning_service DIRECTORY warning_msgs)
generate_opp_message(src/msgs/ReqVR.msg TARGET warning_service DIRECTORY warning_msgs)
generate_opp_message(src/msgs/OBUBeacon.msg TARGET warning_service DIRECTORY warning_msgs)
generate_opp_message(src/msgs/WarningValidationMessage.msg TARGET warning_service DIRECTORY warning_msgs)
add_opp_run(warning_service CONFIG omnetpp.ini NED_FOLDERS src/apps/)

if(WITH_ENVMOD)
    target_link_libraries(warning_service PUBLIC envmod)
endif()