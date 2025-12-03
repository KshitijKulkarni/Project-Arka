import 'package:flutter/material.dart';
import 'debug_draggable.dart';
import 'package:flutter_glow/flutter_glow.dart';
import 'colortheme.dart';
import 'dart:math';

class Temperatures extends StatefulWidget {
  const Temperatures({super.key});

  @override
  State<Temperatures> createState() => _TemperaturesState();
}

class _TemperaturesState extends State<Temperatures> with SingleTickerProviderStateMixin {
  int _selectedTier = 1; // 1 for inner, 2 for mid, 3 for outer
  late AnimationController _controller;
  late Animation<double> _animation;
  final double _deselectedOpacity = 0.05; // Tunable variable for deselected opacity

  @override
  void initState() {
    super.initState();
    _controller = AnimationController(
      duration: const Duration(seconds: 10), // Adjust duration for rotation speed
      vsync: this,
    )..repeat();
    _animation = Tween<double>(begin: 0, end: 1).animate(_controller);
  }

  @override
  void dispose() {
    _controller.dispose();
    super.dispose();
  }

  void _handleTierChange(int? value) {
    if (value != null) {
      setState(() {
        _selectedTier = value;
      });
    }
  }

  @override
  Widget build(BuildContext context) {
    return Stack(
      children: [
        // Inner tier radio buttons
        // Inner-Top
        Positioned(
          top: 56,
          left: 204,
          child: Radio(
            value: 1,
            groupValue: _selectedTier,
            onChanged: _handleTierChange,
            activeColor: Color(0xFF2A2A2A),
            innerRadius: WidgetStatePropertyAll(13),
            backgroundColor: WidgetStatePropertyAll(ora),
            splashRadius: 10,
          ),
        ),
        // Inner-Bottom-Right
        Positioned(
          top: 228,
          left: 264,
          child: Radio(
            value: 1,
            groupValue: _selectedTier,
            onChanged: _handleTierChange,
            activeColor: Color(0xFF2A2A2A),
            innerRadius: WidgetStatePropertyAll(13),
            backgroundColor: WidgetStatePropertyAll(ora),
            splashRadius: 10,
          ),
        ),
        // Inner-Bottom-Left
        Positioned(
          top: 194,
          left: 85,
          child: Radio(
            value: 1,
            groupValue: _selectedTier,
            onChanged: _handleTierChange,
            activeColor: Color(0xFF2A2A2A),
            innerRadius: WidgetStatePropertyAll(13),
            backgroundColor: WidgetStatePropertyAll(ora),
            splashRadius: 10,
          ),
        ),

        // Middle tier radio buttons
        // Mid-Bottom-Left
        Positioned(
          top: 212,
          left: 67,
          child: Radio(
            value: 2,
            groupValue: _selectedTier,
            onChanged: _handleTierChange,
            activeColor: Color(0xFF2A2A2A),
            innerRadius: WidgetStatePropertyAll(13),
            backgroundColor: WidgetStatePropertyAll(ora),
            splashRadius: 10,
          ),
        ),
        // Mid-Top
        Positioned(
          top: 32,
          left: 197,
          child: Radio(
            value: 2,
            groupValue: _selectedTier,
            onChanged: _handleTierChange,
            activeColor: Color(0xFF2A2A2A),
            innerRadius: WidgetStatePropertyAll(13),
            backgroundColor: WidgetStatePropertyAll(ora),
            splashRadius: 10,
          ),
        ),
        // Mid-Bottom-Right
        Positioned(
          top: 234.5,
          left: 288,
          child: Radio(
            value: 2,
            groupValue: _selectedTier,
            onChanged: _handleTierChange,
            activeColor: Color(0xFF2A2A2A),
            innerRadius: WidgetStatePropertyAll(13),
            backgroundColor: WidgetStatePropertyAll(ora),
            splashRadius: 10,
          ),
        ),

        // Outer tier radio buttons
        // Outer-Bottom-Right
        Positioned(
          top: 241,
          left: 312,
          child: Radio(
            value: 3,
            groupValue: _selectedTier,
            onChanged: _handleTierChange,
            activeColor: Color(0xFF2A2A2A),
            innerRadius: WidgetStatePropertyAll(13),
            backgroundColor: WidgetStatePropertyAll(ora),
            splashRadius: 10,
          ),
        ),
        // Outer-Bottom-Left
        Positioned(
          top: 229,
          left: 49,
          child: Radio(
            value: 3,
            groupValue: _selectedTier,
            onChanged: _handleTierChange,
            activeColor: Color(0xFF2A2A2A),
            innerRadius: WidgetStatePropertyAll(13),
            backgroundColor: WidgetStatePropertyAll(ora),
            splashRadius: 10,
          ),
        ),
        // Outer-Top
        Positioned(
          top: 7.5,
          left: 191,
          child: Radio(
            value: 3,
            groupValue: _selectedTier,
            onChanged: _handleTierChange,
            activeColor: Color(0xFF2A2A2A),
            innerRadius: WidgetStatePropertyAll(13),
            backgroundColor: WidgetStatePropertyAll(ora),
            splashRadius: 10,
          ),
        ),

        //
        Positioned(
          top: 97,
          left: 165,
          child: GlowText(
            "1342",
            style: TextStyle(fontFamily: "Infynite", color: yel, fontSize: 28),
          ),
        ),
        Positioned(
          top: 169,
          left: 210,
          child: GlowText(
            "1342",
            style: TextStyle(fontFamily: "Infynite", color: yel, fontSize: 28),
          ),
        ),
        Positioned(
          top: 169,
          left: 119,
          child: GlowText(
            "1342",
            style: TextStyle(fontFamily: "Infynite", color: yel, fontSize: 28),
          ),
        ),
        Positioned(
          top: 218,
          left: 165,
          child: GlowText(
            "Kelvin",
            style: TextStyle(fontFamily: "Infynite", color: ora, fontSize: 28),
          ),
        ),

        // Conditionally display circles based on selected tier with fade animation
        Positioned(
          top: 19,
          left: 46,
          child: AnimatedOpacity(
            opacity: _selectedTier == 3 ? 1.0 : _deselectedOpacity,
            duration: const Duration(milliseconds: 300),
            child: RotationTransition(
              turns: _animation,
              child: IgnorePointer(
                child: SizedBox(
                  width: 310,
                  child: Image.asset("assets/L.png", fit: BoxFit.fitWidth),
                ),
              ),
            ),
          ),
        ),
        Positioned(
          top: 45,
          left: 68,
          child: AnimatedOpacity(
            opacity: _selectedTier == 2 ? 1.0 : _deselectedOpacity,
            duration: const Duration(milliseconds: 300),
            child: RotationTransition(
              turns: _animation,
              child: IgnorePointer(
                child: SizedBox(
                  width: 262,
                  child: Image.asset("assets/M.png", fit: BoxFit.fitWidth),
                ),
              ),
            ),
          ),
        ),
        Positioned(
          top: 65,
          left: 92,
          child: AnimatedOpacity(
            opacity: _selectedTier == 1 ? 1.0 : _deselectedOpacity,
            duration: const Duration(milliseconds: 300),
            child: RotationTransition(
              turns: _animation,
              child: IgnorePointer(
                child: SizedBox(
                  width: 217,
                  child: Image.asset("assets/S.png", fit: BoxFit.fitWidth),
                ),
              ),
            ),
          ),
        ),
        // DebugDraggable(child: SizedBox(width: 217, child: Image.asset("assets/S.png", fit: BoxFit.fitWidth))),
      ],
    );
  }
}
