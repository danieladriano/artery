import artery.application.ItsG5Service;

simple RsuWarningService like ItsG5Service {
    parameters:
        bool withReputation = default(true);
        @signal[warning](type="long");
        @statistic[warningMessageRSU](title="Received WM"; source="warning"; record=vector; interpolationmode=none);
}
