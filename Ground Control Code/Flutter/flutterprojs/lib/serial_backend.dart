import 'dart:async';
import 'dart:ffi';
import 'dart:typed_data';
import 'package:flutter/material.dart';
import 'package:flutter_libserialport/flutter_libserialport.dart';

late Map<String, double> data;

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

  Map<String, double> get dataChunk => data;

  SerialBackend() {
    _init();
  }

  void _init() {
    data = {
      "rp100ms": 0.0,
      "thrust": 0.0,
      "currentFlow": 0.0,
      "targetFlow": 0.0,
      "p1": 0.0,
      "p2": 0.0,
      "p3": 0.0,
      "p4": 0.0,
      "p5": 0.0,
      "t1": 0.0,
      "t2": 1.0,
      "t3": 0.0,
      "t4": 0.0,
      "t5": 0.0,
      "t6": 0.0,
      "t7": 0.0,
      "t8": 0.0,
      "t9": 0.0,
      "status": 0.0,
    };
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
    //Bytes order
    /*
    rp100ms
    engineThrust
    currentFlowRate
    targetFlowRate
    fuel
    tempU
    tempL
    tempR
     */
    serialPort.flush();
    serialPort.write(Uint8List.fromList([97]), timeout: 10); //Send a request
    Uint8List dataIn = serialPort.read(35, timeout: 10);
    if(dataIn.length == 34) {
      print("Updated");
      print(dataIn.toString());
      // status is the first int (index 0)
      // status is the first int (index 0) - (1-byte, no endianness change)
      data['status'] = (dataIn[0]).toDouble();

      // rp100ms - (1-byte, no endianness change)
      data['rp100ms'] = dataIn[1].toDouble();

      // thrust (16-bit, now Big Endian: dataIn[MSB] << 8 | dataIn[LSB])
      // Uses dataIn[3] and dataIn[2]
      data['thrust'] = ((dataIn[3] << 8) | dataIn[2]).toDouble();

      // currentFlow & targetFlow - (1-byte, no endianness change)
      data['currentFlow'] = (dataIn[4]).toDouble();
      data['targetFlow'] = (dataIn[5]).toDouble();

      // 16-bit Pressure values (p1 - p5) - Now Big Endian
      // p1 uses dataIn[7] and dataIn[6]
      data['p1'] = ((dataIn[7] << 8) | dataIn[6]).toDouble();
      data['p2'] = ((dataIn[9] << 8) | dataIn[8]).toDouble();
      data['p3'] = ((dataIn[11] << 8) | dataIn[10]).toDouble();
      data['p4'] = ((dataIn[13] << 8) | dataIn[12]).toDouble();
      data['p5'] = ((dataIn[15] << 8) | dataIn[14]).toDouble();

      // 16-bit Temperature values (t1 - t9) - Now Big Endian
      // t1 uses dataIn[17] and dataIn[16]
      data['t1'] = ((dataIn[17] << 8) | dataIn[16]).toDouble();
      data['t2'] = ((dataIn[19] << 8) | dataIn[18]).toDouble();
      data['t3'] = ((dataIn[21] << 8) | dataIn[20]).toDouble();
      data['t4'] = ((dataIn[23] << 8) | dataIn[22]).toDouble();
      data['t5'] = ((dataIn[25] << 8) | dataIn[24]).toDouble();
      data['t6'] = ((dataIn[27] << 8) | dataIn[26]).toDouble();
      data['t7'] = ((dataIn[29] << 8) | dataIn[28]).toDouble();
      data['t8'] = ((dataIn[31] << 8) | dataIn[30]).toDouble();
      data['t9'] = ((dataIn[33] << 8) | dataIn[32]).toDouble();

      print(data.toString());
    }
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
