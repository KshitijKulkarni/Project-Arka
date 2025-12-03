import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'colortheme.dart';
import 'serial_backend.dart';

class SerialSetup extends StatelessWidget {
   SerialSetup({super.key});

  @override
  Widget build(BuildContext context) {
    final backend = Provider.of<SerialBackend>(context);
    return Column(
      spacing: backend.connectionStatus == 2 ? 0 : 10,
      mainAxisAlignment: MainAxisAlignment.center,
      children: [
        Container(
          decoration: ShapeDecoration(
            shape: BeveledRectangleBorder(
              side: BorderSide(width: 1, color: grn),
              borderRadius: BorderRadius.circular(10),
            ),
          ),
          padding:  EdgeInsets.fromLTRB(10, 0, 0, 0),
          child: DropdownButtonHideUnderline(
            child: DropdownButton<String>(
              elevation: 5,
              dropdownColor:  Color(0xFF303030),
              hint:  Text(
                "Select Port",
                style: TextStyle(
                  fontFamily: "Infynite",
                  color: grn,
                  fontSize: 20,
                ),
              ),
              value: backend.selectedPort,
              items: backend.availablePorts
                  .map(
                    (e) => DropdownMenuItem(
                      value: e,
                      child: Text(
                        e,
                        style:  TextStyle(
                          fontFamily: "Infynite",
                          color: grn,
                          fontSize: 20,
                        ),
                      ),
                    ),
                  )
                  .toList(),
              onChanged: (val) {
                backend.selectedPort = val;
              },
            ),
          ),
        ),
        Container(
          decoration: ShapeDecoration(
            shape: BeveledRectangleBorder(
              side: BorderSide(width: 1, color: grn),
              borderRadius: BorderRadius.circular(10),
            ),
          ),
          padding:  EdgeInsets.fromLTRB(10, 0, 0, 0),
          child: DropdownButtonHideUnderline(
            child: DropdownButton<int>(
              elevation: 5,
              dropdownColor:  Color(0xFF303030),
              hint:  Text(
                "Select Baud Rate",
                style: TextStyle(
                  fontFamily: "Infynite",
                  color: grn,
                  fontSize: 20,
                ),
              ),
              value: backend.selectedBaudRate,
              items: [1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200]
                  .map(
                    (e) => DropdownMenuItem(
                      value: e,
                      child: Text(
                        e.toString(),
                        style:  TextStyle(
                          fontFamily: "Infynite",
                          color: grn,
                          fontSize: 20,
                        ),
                      ),
                    ),
                  )
                  .toList(),
              onChanged: (val) {
                backend.selectedBaudRate = val;
              },
            ),
          ),
        ),
        ElevatedButton(
          onPressed: () {
            if (backend.connectionStatus == 1) {
              backend.disconnect();
            } else {
              backend.connect();
            }
          },
          style: ElevatedButton.styleFrom(
            backgroundColor: backend.connectionStatus == 1 ? ora : grn,
            shape: BeveledRectangleBorder(
              borderRadius: BorderRadius.circular(7),
            ),
          ),
          child: Text(backend.connectionStatus == 1 ? "Disconnect" : "Connect"),
        ),
        if (backend.connectionStatus == 2)
          Text(
            backend.errorString!,
            style:  TextStyle(
              color: Colors.red,
              fontFamily: "Infynite",
              fontSize: 15,
              // height: 0.5,
            ),
          ),
        SizedBox(height: backend.connectionStatus == 2 ? 0 : 10,)
      ],
    );
  }
}
