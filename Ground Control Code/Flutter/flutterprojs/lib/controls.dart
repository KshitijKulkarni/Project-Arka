import 'dart:io';

import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:window_manager/window_manager.dart';
import 'debug_draggable.dart';
import 'colortheme.dart';
import 'package:flutter_glow/flutter_glow.dart';
import 'plots.dart';
import 'serial_setup.dart';
import 'pressures.dart';
import 'temperatures.dart';
import 'readouts.dart';

class Control extends StatefulWidget {
  const Control({super.key});

  @override
  State<Control> createState() => _ControlState();
}

class _ControlState extends State<Control> {
  double hintOpac = 0;

  void showHint() {
    setState(() {
      hintOpac = 1;
    });
    Future.delayed(Duration(seconds: 3), () {
      setState(() {
        hintOpac = 0;
      });
    });
  }

  @override
  Widget build(BuildContext context) {
    return Stack(
      children: [
        Image.asset("assets/BGNew.png", fit: BoxFit.fill),
        Column(
          children: [
            DragToMoveArea(child: SizedBox(width: double.infinity, height: 55)),
            SizedBox(
              height: 235,
              width: double.infinity,
              child: Row(
                children: [
                  SizedBox(height: double.infinity, width: 165),
                  SizedBox(width: 63, height: double.infinity),
                  SizedBox(
                    height: double.infinity,
                    width: 610,
                    child: Padding(
                      padding: const EdgeInsets.fromLTRB(30.0, 0, 30, 30),
                      child: Plots(),
                    ),
                  ),
                  SizedBox(width: 76, height: double.infinity),
                  SizedBox(
                    height: double.infinity,
                    width: 466,
                    child: Padding(
                      padding: const EdgeInsets.fromLTRB(40, 0, 40, 40),
                      child: SerialSetup(),
                    ),
                  ),
                ],
              ),
            ),
            SizedBox(height: 95),
            SizedBox(
              height: 373,
              width: double.infinity,
              child: Row(
                children: [
                  SizedBox(height: double.infinity, width: 165),
                  SizedBox(width: 61, height: double.infinity),
                  SizedBox(
                    height: double.infinity,
                    width: 498,
                    child: Padding(
                      padding: const EdgeInsets.all(8.0),
                      child: Pressures(),
                    ),
                  ),
                  SizedBox(width: 22, height: double.infinity),
                  SizedBox(
                    height: double.infinity,
                    width: 400,
                    child: Temperatures(),
                  ),
                  SizedBox(width: 22, height: double.infinity),
                  SizedBox(
                    height: double.infinity,
                    width: 250,
                    child: Padding(
                      padding: const EdgeInsets.fromLTRB(15, 35, 15, 8),
                      child: Readouts(),
                    ),
                  ),
                ],
              ),
            ),
          ],
        ),
        Positioned(
          top: 650,
          left: 30,
          child: AnimatedOpacity(
            opacity: hintOpac,
            duration: Duration(milliseconds: 100),
            child: Text(
              "Hold down.",
              style: TextStyle(
                fontFamily: "Infynite",
                color: Color(0xFFEA1504),
                fontSize: 28,
              ),
            ),
          ),
        ),
        Positioned(
          top: 706,
          left: 51,
          child: TextButton(
            onPressed: () {
              showHint();
            },
            onLongPress: () {
              exit(0);
            },
            style: TextButton.styleFrom(
              backgroundColor: Color(0xFF920B00),

              shape: CircleBorder(),
            ),
            child: Text(""),
          ),
        ),
      ],
    );
  }
}
