struct Reputation{
    string idVehicle;
    double reputation;
};

packet UpdateTableMessage
{
    string idRsu;
    Reputation reputationTable[];
    double tableTime;
};