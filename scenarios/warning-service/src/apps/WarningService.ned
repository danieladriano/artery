import artery.application.ItsG5Service;

simple WarningService like ItsG5Service {
    parameters:
        bool withReputation = default(true);
        @signal[auxTravelTime](type="long");
        @statistics[time](title="Travel Time"; source="auxTravelTime"; record=vector,stats; interpolationmode=none);
}