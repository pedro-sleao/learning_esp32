from Comunicacao import MQTTCommunicator
from matplotlib import pyplot as plt
import logging
from collections import deque

logging.basicConfig(level=logging.INFO)

comms = MQTTCommunicator("receive_data", "Endereco do broker")
comms.start()

comms.subscribe("esp_data")

num_pontos = 800
data = deque([0] * num_pontos, maxlen=num_pontos)
tempo = list(range(num_pontos))

plt.ion()

figure, ax = plt.subplots()

line, = plt.plot(tempo, list(data))
plt.xlim(0, num_pontos)
plt.ylim(0, 4500)

while True:
    if comms.info:
        novo_dado = int(comms.info)
        data.append(novo_dado)
        line.set_ydata(list(data))
        figure.canvas.draw()
        figure.canvas.flush_events()
