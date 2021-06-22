import artery.application.ItsG5Service;

simple WarningService like ItsG5Service {
    parameters:
        @signal[mTravelTime](type="long");
        @statistic[mainTravelTime](title="Main Travel Time"; source="mTravelTime"; record=vector,stats; interpolationmode=none);

        @signal[aTravelTime](type="long");
        @statistic[auxTravelTime](title="Aux Travel Time"; source="aTravelTime"; record=vector,stats; interpolationmode=none);

        @signal[change](type="long");
        @statistic[changeRoute](title="Change Route"; source="change"; record=vector; interpolationmode=none);

        @signal[receivedWM](type="long");
        @statistic[receivedWarningMessage](title="Received Warning Message"; source="receivedWM"; record=vector; interpolationmode=none);
}