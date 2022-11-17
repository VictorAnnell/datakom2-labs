NS3=/it/kurs/datakom2/lab1/ns3-run

tcp:
	$(NS3) sim-tcp.py --latency=10 --rate=100000
udp:
	$(NS3) sim-udp.py --latency=1
