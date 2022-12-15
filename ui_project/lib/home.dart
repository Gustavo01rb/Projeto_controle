import 'package:charts_flutter/flutter.dart' as charts;
import 'package:flutter/material.dart';
import 'package:ui_project/config.dart';
import 'package:ui_project/core/colors.dart';
import 'package:ui_project/waitpage.dart';
import 'package:web_socket_channel/io.dart';

import 'button.dart';
import 'chart.dart';

class Home extends StatefulWidget {
  const Home({super.key});

  @override
  State<Home> createState() => _HomeState();
}

class _HomeState extends State<Home> {
  late bool connected;
  late IOWebSocketChannel channel;
  late bool activeMotor;
  late double _currentSliderValue;
  late int rpm_motor = 0;
  List<int> rpmList = [0];

  @override
  void initState() {
    connected = false;
    activeMotor = false;
    _currentSliderValue = 20;
    rpm_motor = 0;

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
          } else {
            setState(() {
              rpm_motor = int.parse(message);
              rpmList.add(rpm_motor);
            });
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

  Future<void> sendSpeed(double value) async {
    if (!activeMotor) return;
    channel.sink.add("true;${((_currentSliderValue * 127) / 400).round()}");
  }

  void onButtonPressed() {
    setState(() => activeMotor = !activeMotor);
    channel.sink.add("$activeMotor;$_currentSliderValue");
  }

  void onChangedSlider(double value) =>
      setState(() => _currentSliderValue = value);

  @override
  Widget build(BuildContext context) {
    final double width = MediaQuery.of(context).size.width;
    return !connected
        ? const WaitPage()
        : Scaffold(
            appBar: AppBar(
              centerTitle: true,
              backgroundColor: ProjectColors.darkBlue,
              title: const Text("Controle Tensão Motor"),
            ),
            backgroundColor: ProjectColors.black,
            body: SingleChildScrollView(
              child: Column(
                  mainAxisAlignment: MainAxisAlignment.center,
                  children: [
                    SizedBox(
                      width: width,
                      height: 15,
                    ),
                    SizedBox(
                      width: 300,
                      height: 300,
                      child: Card(
                          color: ProjectColors.white,
                          child: ChartRPM(_createSampleData(rpmList))),
                    ),
                    const SizedBox(
                      height: 15,
                    ),
                    const Text("Ajustador de tensão",
                        style: TextStyle(
                          fontWeight: FontWeight.w400,
                          fontSize: 18,
                          color: Colors.white,
                        )),
                    SizedBox(
                      width: width * 0.8,
                      child: Padding(
                        padding: const EdgeInsets.all(50.0),
                        child: Slider(
                            value: _currentSliderValue,
                            max: 400,
                            divisions: 127,
                            onChangeEnd: (value) async => sendSpeed(value),
                            onChanged: onChangedSlider),
                      ),
                    ),
                    Padding(
                      padding: const EdgeInsets.only(top: 20.0),
                      child: ButtonDefault(
                        width: width * 0.6,
                        onPressed: onButtonPressed,
                        text: activeMotor ? "Desligar Motor" : "Ligar motor",
                        color: !activeMotor ? ProjectColors.blue : Colors.red,
                      ),
                    ),
                    Padding(
                      padding: const EdgeInsets.all(50.0),
                      child: Text("Rotação do motor $rpm_motor rpm",
                          style: const TextStyle(
                            fontWeight: FontWeight.w400,
                            fontSize: 18,
                            color: Colors.white,
                          )),
                    )
                  ]),
            ));
  }

  static List<charts.Series<Chart, int>> _createSampleData(List<int> rpmList) {
    final List<Chart> list = [];

    for (int valor = 0; valor < rpmList.length; valor++) {
      list.add(Chart(valor, rpmList[valor]));
    }

    return [
      charts.Series<Chart, int>(
        id: 'Gráfico tensão por tempo',
        colorFn: (_, __) => charts.MaterialPalette.purple.shadeDefault,
        domainFn: (Chart value, _) => value.time,
        measureFn: (Chart value, _) => value.rpm,
        data: list,
      )
    ];
  }
}
