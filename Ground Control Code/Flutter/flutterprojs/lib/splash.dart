import 'package:flutter/material.dart';
import 'package:video_player/video_player.dart';
// import 'package:video_player_win/video_player_win.dart';
// import 'package:halo_videoplayer/halo_videoplayer.dart';

class SplashScreen extends StatefulWidget {
  const SplashScreen({super.key});

  @override
  State<SplashScreen> createState() => _SplashScreenState();
}

class _SplashScreenState extends State<SplashScreen> {
  late VideoPlayerController _controller;

  @override
  void initState() {
    // TODO: implement initState
    super.initState();
    initVideoPlayer();
  }

  void initVideoPlayer() async {
    _controller = VideoPlayerController.asset("assets/splash.mp4");
    _controller.initialize().then((_) {
      setState(() {});
    });
    _controller.play();
  }

  @override
  void dispose() {
    // TODO: implement dispose
    super.dispose();
    _controller.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: Center(
        child: AspectRatio(
          aspectRatio: _controller.value.aspectRatio,
          child: VideoPlayer(_controller),
        ),
      ),
    );
  }
}
