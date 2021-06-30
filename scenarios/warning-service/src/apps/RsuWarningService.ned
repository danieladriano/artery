import artery.application.ItsG5Service;

simple RsuWarningService like ItsG5Service {
    parameters:
        bool withReputation = default(false);

        @signal[warning](type="long");
        @statistic[warningMessageRSU](title="Received WM"; source="warning"; record=vector; interpolationmode=none);

        @signal[beacon](type="long");
        @statistic[obuBeacon](title="Received WM"; source="beacon"; record=vector; interpolationmode=none);
}
