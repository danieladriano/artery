from models import connection
from peewee import IntegerField, TextField, DoubleField, fn

class Vector(connection.BaseModel):
    vectorId = IntegerField()
    runId = IntegerField()
    moduleName = TextField()
    vectorName = TextField()
    vectorCount = IntegerField()
    vectorMin = DoubleField()
    vectorMax = DoubleField()
    vectorSum = DoubleField()
    vectorSumSqr = DoubleField()
    startEventNum = IntegerField()
    endEventNum = IntegerField()
    startSimtimeRaw = IntegerField()
    endSimtimeRar = IntegerField()

    class Meta:
        table_name = "vector"

def get_avg(vectorName: str):
    try:
        return Vector.select(fn.AVG(Vector.vectorMax).alias("avg")).where(Vector.vectorName % f"*{vectorName}*").get()
    except Vector.DoesNotExist:
        return None
    except Exception as ex:
        raise ex

def get_count(vectorName: str):
    try:
        return Vector.select(fn.COUNT(Vector.vectorMax).alias("count")).where(Vector.vectorName % f"*{vectorName}*").get()
    except Vector.DoesNotExist:
        return None
    except Exception as ex:
        raise ex
