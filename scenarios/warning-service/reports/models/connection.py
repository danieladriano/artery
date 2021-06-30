from peewee import SqliteDatabase, Model


dbpath = "../results/Scenario_1_Muito_Esparso-#0.vec"
# dbpath = "../results/scenario_1/Scenario_1_Esparso-#0.vec"
# dbpath = "../results/scenario_1/Scenario_1_Normal-#1.vec"
# dbpath = "../results/scenario_1/Scenario_1_Denso-#0.vec"
# dbpath = "../results/scenario_1/Scenario_1_Muito_Denso-#0.vec"

database = SqliteDatabase(dbpath, pragmas={
    'foreign_keys': 1,
})

class BaseModel(Model):
    class Meta:
        database = database
