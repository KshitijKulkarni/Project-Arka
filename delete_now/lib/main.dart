import 'package:flutter/material.dart';

void main() {
  runApp(MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Flutter Demo',
      theme: ThemeData(

        colorScheme: ColorScheme.fromSeed(seedColor: Colors.deepPurple),
      ),
      home: Home()
    );
  }
}

class Home extends StatefulWidget {
  const Home({super.key});

  @override
  State<Home> createState() => _HomeState();
}

class _HomeState extends State<Home> {

  bool show = true;

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(backgroundColor: Colors.blue[600],
      title: Text("Hello World"),
      ),
      body: Center(
        child: Column(
          spacing: 8,
          children: [
            TextButton(onPressed: (){}, child: Text("Press me"), style: TextButton.styleFrom(
              backgroundColor: Colors.lightGreenAccent
            ),),
            TextButton(onPressed: (){}, child: Text("Press me"), style: TextButton.styleFrom(
                backgroundColor: Colors.lightGreenAccent
            ),),
            TextButton(onPressed: (){
              setState(() {
                show =! show;
              });
            }, child: Text("Press me"), style: TextButton.styleFrom(
                backgroundColor: show ? Colors.lightGreenAccent : Colors.red
            ),),
          ],
        ),
      ),
    );
  }
}

