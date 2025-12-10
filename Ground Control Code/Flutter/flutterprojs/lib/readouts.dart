import 'package:flutter/material.dart';
import 'package:flutter_glow/flutter_glow.dart';
import 'package:flutterprojs/serial_backend.dart';
import 'package:provider/provider.dart';
import 'colortheme.dart';

class Readouts extends StatefulWidget {
  const Readouts({super.key});

  @override
  State<Readouts> createState() => _ReadoutsState();
}

class _ReadoutsState extends State<Readouts> {
  @override
  Widget build(BuildContext context) {
    final backend = Provider.of<SerialBackend>(context);
    return Column(
      spacing: 0,
      children: [
        SizedBox(height: 23),
        GlowText(
          "Fuel Flow",
          style: TextStyle(
            fontFamily: "Infynite",
            color: yel,
            fontSize: 25,
            height: 0.8,
          ),
        ),
        GlowText(
          "ml/min",
          style: TextStyle(
            fontFamily: "Infynite",
            color: yel.withAlpha(200),
            fontSize: 17,
          ),
        ),
        SizedBox(height: 20),
        GlowText(
          backend.dataChunk['currentFlow']!.toString(),
          style: TextStyle(
            fontFamily: "Infynite",
            color: yel,
            fontSize: 50,
            height: 0.8,
          ),
        ),
        GlowText(
          backend.dataChunk['targetFlow']!.toString(),
          style: TextStyle(fontFamily: "Infynite", color: grn, fontSize: 25),
        ),

        Spacer(),
        Row(
          children: [
            SizedBox(width: 10),
            SizedBox(
              width: 85,
              height: 90,
              child: Center(
                child: Column(
                  children: [
                    SizedBox(height: 5,),
                    GlowText(
                      "Thrust",
                      style: TextStyle(
                        fontFamily: "Infynite",
                        color: yel,
                        fontSize: 18,
                      ),
                    ),
                    GlowText(
                      backend.dataChunk['thrust']!.toString(),
                      style: TextStyle(
                        fontFamily: "Infynite",
                        color: yel,
                        fontSize: 25,
                      ),
                    ),
                  ],
                ),
              ),
            ),
            Spacer(),
            SizedBox(
              width: 85,
              height: 90,
              child: Center(
                child: Column(
                  children: [
                    SizedBox(height: 5,),
                    GlowText(
                      "Speed",
                      style: TextStyle(
                        fontFamily: "Infynite",
                        color: yel,
                        fontSize: 18,
                      ),
                    ),
                    GlowText(
                      backend.dataChunk['rp100ms']!.toString(),
                      style: TextStyle(
                        fontFamily: "Infynite",
                        color: yel,
                        fontSize: 25,
                      ),
                    ),
                  ],
                ),
              ),
            ),

            SizedBox(width: 8),
          ],
        ),
        SizedBox(height: 10),
      ],
    );
  }
}
