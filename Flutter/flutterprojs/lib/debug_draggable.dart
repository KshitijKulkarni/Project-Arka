import "package:flutter/material.dart";

class DebugDraggable extends StatefulWidget {
  final Widget child;
  final double initialTop;
  final double initialLeft;

  const DebugDraggable({
    super.key,
    required this.child,
    this.initialTop = 0,
    this.initialLeft = 0,
  });

  @override
  State<DebugDraggable> createState() => _DebugDraggableState();
}

class _DebugDraggableState extends State<DebugDraggable> {
  late double top;
  late double left;

  @override
  void initState() {
    super.initState();
    top = widget.initialTop;
    left = widget.initialLeft;
  }

  @override
  Widget build(BuildContext context) {
    return Positioned(
      top: top,
      left: left,
      child: GestureDetector(
        onPanUpdate: (details) {
          setState(() {
            top += details.delta.dy;
            left += details.delta.dx;
          });
        },
        onPanEnd: (details) {
          // Prints the exact code to copy-paste!
          print("Positioned(top: $top, left: $left, child: ...)");
        },
        child: Container(
          decoration: BoxDecoration(
            border: Border.all(
              color: Colors.red,
              width: 2,
            ), // Visibility helper
          ),
          child: widget.child,
        ),
      ),
    );
  }
}
