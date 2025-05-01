from dateutil import parser
import threading
import datetime
import socket
import time


class ClockServer:
	def __init__(self, port=8080):
		self.port = port
		self.clients = {}
		self.server_socket = socket.socket()
		self.server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
		self.server_socket.bind(('localhost', self.port))
		self.server_socket.listen(10)

	def start(self):
		threading.Thread(target=self._accept_clients).start()
		threading.Thread(target=self._sync_clocks).start()

	def _accept_clients(self):
		while True:
			client_socket, client_address = self.server_socket.accept()
			print(f"Client connected: {client_address}")
			self.clients[client_address] = {'socket': client_socket, 'time_diff': None}
			threading.Thread(target=self._receive_time, args=(client_address,)).start()

	def _receive_time(self, client_address):
		while True:
			try:
				client_data = self.clients[client_address]
				clock_time_string = client_data['socket'].recv(1024).decode()
				client_time = parser.parse(clock_time_string)
				client_data['time_diff'] = datetime.datetime.now() - client_time
				print(f"Time received from: {client_address}")
			except Exception as e:
				print(f"Error receiving time from {client_address}: {e}")
				del self.clients[client_address]
				break

	def _sync_clocks(self):
		while True:
			if len(self.clients) > 0:
				time_diffs = [client['time_diff'] for client in self.clients.values() if client['time_diff'] is not None]
				avg_time_diff = sum(time_diffs, datetime.timedelta()) / len(time_diffs) if time_diffs else datetime.timedelta()
				synchronized_time = datetime.datetime.now() + avg_time_diff
				for client_address, client_data in self.clients.items():
					try:
						client_data['socket'].send(str(synchronized_time).encode())
						print(f"Synchronized time sent to {client_address}")
					except Exception as e:
						print(f"Error sending synchronized time to {client_address}: {e}")
						del self.clients[client_address]
			time.sleep(5)


if __name__ == '__main__':
	server = ClockServer()
	server.start()
