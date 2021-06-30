from models import connection, vector


COMPRIMENTO_TRAJETO_P = 5086.77
COMPRIMENTO_TRAJETO_A = 8125.21


def main():
    connection.database.connect(reuse_if_open=True)

    tempo_medio_trajeto_principal = vector.get_avg("mainTravelTime")
    tempo_medio_trajeto_alternativo = vector.get_avg("auxTravelTime")
    total_veiculos = vector.get_count("totalVehiclesSim")
    total_veiculos_trecho = vector.get_count("totalVehiclesHighwaySim")
    receberam_alerta = vector.get_count("receivedWarningMessage")
    alteraram_roteito = vector.get_count("changeRoute")

    rsus_receberam_alerta = vector.get_count("warningMessageRSU")

    try:
        print(f"Tempo médio trajeto principal {tempo_medio_trajeto_principal.avg}")
        velocidade_media_trajeto_principal = (COMPRIMENTO_TRAJETO_P / tempo_medio_trajeto_principal.avg) * 3.632
        print(f"Velocidade media trajeto principal {velocidade_media_trajeto_principal} km/h")
    except:
        pass

    try:
        print(f"Tempo médio trajeto alternativo {tempo_medio_trajeto_alternativo.avg}")
        velocidade_media_trajeto_alternativo = (COMPRIMENTO_TRAJETO_A / tempo_medio_trajeto_alternativo.avg) * 3.632
        print(f"Velocidade media trajeto alternativo {velocidade_media_trajeto_alternativo} km/h")
    except:
        pass

    print(f"Total de veiculos na simulacao {total_veiculos.count}")
    print(f"Total de veiculos no trecho de interesse {total_veiculos_trecho.count}")
    print(f"Total de veiculos que receberam o alerta {receberam_alerta.count}")
    print(f"Total de veiculos que alteraram o roteiro {alteraram_roteito.count}")
    print(f"total de RSUs que receberam o alerta {rsus_receberam_alerta.count}")
    
    

main()


# 610,80
# 99,97
# 284,49
# 100,00
# 1298,60
# 100,00
# 1329,97
# 100,00
# 326,54
# 99,94
# 636,47
# 99,99
# total -> 5086,77


# 610,80
# 99,97
# 284,49
# 100,00
# 374,12
# 179,35
# 476,87
# 100,00
# 63,52
# 64,22
# 100,75
# 527,76
# 69,56
# 52,21
# 88,13
# 91,51
# 18,49
# 28,79
# 169,04
# 125,40
# 63,19
# 120,46
# 42,02
# 150,27
# 49,48
# 129,66
# 165,36
# 31,47
# 82,26
# 75,02
# 112,08
# 294,33
# 316,30
# 392,48
# 359,65
# 99,94
# 1094,89
# 99,99
# 152,36
# 266,11
# 302,92
# 99,99
# total -> 8125,21
