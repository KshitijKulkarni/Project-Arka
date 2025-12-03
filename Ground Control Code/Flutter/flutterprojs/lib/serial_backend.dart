import 'dart:async';
import 'package:flutter/material.dart';
import 'package:flutter_libserialport/flutter_libserialport.dart';

class DataChunk {
  double currentFlowRate = 0;
  double targetFlowRate = 0;

  double engineThrust = 0;
  double fuel = 0;

  double tempU = 0;
  double tempL = 0;
  double tempR = 0;

  DataChunk(
    this.currentFlowRate,
    this.targetFlowRate,
    this.engineThrust,
    this.fuel,
    this.tempU,
    this.tempL,
    this.tempR,
  );
}

class SerialBackend with ChangeNotifier {
  List<String> _availablePorts = [];

  List<String> get availablePorts => _availablePorts;

  String? _selectedPort;

  String? get selectedPort => _selectedPort;

  set selectedPort(String? value) {
    _selectedPort = value;
    notifyListeners();
  }

  late SerialPort serialPort;

  int? _selectedBaudRate;

  int? get selectedBaudRate => _selectedBaudRate;

  set selectedBaudRate(int? value) {
    _selectedBaudRate = value;
    notifyListeners();
  }

  int _connectionStatus = 0;

  int get connectionStatus => _connectionStatus;

  String? _errorString;

  String? get errorString => _errorString;

  Timer? _portRefreshTimer;

  late DataChunk _dataChunk;

  DataChunk get dataChunk => _dataChunk;

  SerialBackend() {
    _init();
  }

  void _init() {
    _dataChunk = DataChunk(0, 0, 0, 0, 0, 0, 0);
    _refreshPorts();
    _portRefreshTimer = Timer.periodic(const Duration(seconds: 5), (timer) {
      _refreshPorts();
    });
  }

  void _refreshPorts() {
    _availablePorts = SerialPort.availablePorts;
    notifyListeners();
  }

  Future<void> connect() async {
    serialPort = SerialPort(_selectedPort!);
    if (_selectedPort == null || _selectedBaudRate == null) {
      return;
    }

    try {
      if (serialPort.openReadWrite()) {
        _connectionStatus = 1; // Connected
        Timer.periodic(
          Duration(milliseconds: 500),
          (timer) => updateDataChunk(),
        );
      } else {
        _connectionStatus = 2; // Error
        _errorString = "Permission Denied/Port in use";
        notifyListeners();
        await Future.delayed(const Duration(seconds: 2)).then((value) {
          disconnect();
        });
      }
    } catch (e) {
      _errorString = e.toString();
      print(e.toString());
      _connectionStatus = 2; // Error
      notifyListeners();
      await Future.delayed(const Duration(seconds: 2)).then((value) {
        disconnect();
      });
    }
    notifyListeners();
  }

  void updateDataChunk() {
    //TODO: Update data chunk
    print("Updated");
    _dataChunk = DataChunk(100, 100, 100, 100, 100, 100, 100);
    notifyListeners();
  }

  void disconnect() {
    _connectionStatus = 0;
    serialPort.close();
    notifyListeners();
  }

  @override
  void dispose() {
    _portRefreshTimer?.cancel();
    super.dispose();
  }
}
