select count(vectorId) from vector v where vectorName  LIKE '%receivedWarningMessage%';

select count(vectorId) from vector v where vectorName  LIKE '%changeRoute%';

select AVG(vectorMax) from vector v where vectorName  LIKE '%mainTravelTime%';

select AVG(vectorMax) from vector v where vectorName  LIKE '%auxTravelTime%';

select count(vectorId) from vector v  where vectorName  LIKE '%totalVehicles%';

select count(vectorId) from vector v  where vectorName  LIKE 'totalVehiclesHighwaySignal%';

select count(vectorId) as obuBeacon from vector v  where vectorName  LIKE 'obuBeacon%';
select count(vectorId) as warningMessageRSU from vector v  where vectorName  LIKE 'warningMessageRSU%';


