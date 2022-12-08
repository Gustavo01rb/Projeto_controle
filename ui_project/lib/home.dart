import 'package:flutter/material.dart';
import 'package:ui_project/config.dart';
import 'package:ui_project/default_form_with_text.dart';
import 'package:ui_project/waitpage.dart';
import 'package:web_socket_channel/io.dart';

import 'button.dart';

class Home extends StatefulWidget {
  const Home({super.key});

  @override
  State<Home> createState() => _HomeState();
}

class _HomeState extends State<Home> {
  late bool connected;
  late IOWebSocketChannel channel;
  late bool stepMotor;
  final revolutionController = TextEditingController();
  final speedController = TextEditingController();

  @override
  void initState() {
    connected = false;
    stepMotor = false;
    Future.delayed(Duration.zero, () async {
      channelconnect();
    });
    super.initState();
  }

  void channelconnect() {
    try {
      channel = IOWebSocketChannel.connect(
          "ws://${Config.ip}:${Config.webport.toString()}");
      channel.stream.listen((message) {
        setState(() {
          if (message == "connected") {
            debugPrint("Dispositivo conectado");
            connected = true;
          }
        });
      }, onDone: () {
        debugPrint("NODEMCU não conectado.");
        setState(() {
          connected = false;
        });
        channelconnect();
      });
    } catch (_) {
      debugPrint("error on connecting to websocket.");
      channelconnect();
    }
  }

  Future<void> sendcmd() async {
    if (stepMotor) {
      channel.sink.add("false;65;600");
      setState(() {
        stepMotor = false;      
      });
    } else {
      setState(() {
        stepMotor = true;        
      });
      channel.sink.add(
          "true;${revolutionController.text.isEmpty ? 65 : revolutionController.text};${speedController.text.isEmpty ? 600 : speedController.text}");
    }
  }

  @override
  Widget build(BuildContext context) {
    final double width = MediaQuery.of(context).size.width;
    return !connected
        ? const WaitPage()
        : Scaffold(
            body: SafeArea(
              child: Column(
                mainAxisAlignment: MainAxisAlignment.center,
                children: [
                  SizedBox(
                    width: width,
                  ),
                  Text(connected
                      ? "Dispositivo conectado"
                      : "Dispositivo não conectado"),
                  SizedBox(
                      width: width * 0.8,
                      child: FormDefaultWithText(
                        titleText: "Steps per revolution",
                        textInputType: TextInputType.number,
                        controller: revolutionController,
                      )),
                  Padding(
                    padding: const EdgeInsets.symmetric(vertical: 20),
                    child: SizedBox(
                        width: width * 0.8,
                        child: FormDefaultWithText(
                          titleText: "Speed",
                          textInputType: TextInputType.number,
                          controller: speedController,
                        )),
                  ),
                  Padding(
                    padding: const EdgeInsets.only(top: 20.0),
                    child: ButtonDefault(
                      width: width * 0.6,
                      onPressed: () async => sendcmd(),
                      text: stepMotor ? "Desligar Motor" : "Ligar motor",
                    ),
                  )
                ],
              ),
            ),
          );
  }
}
