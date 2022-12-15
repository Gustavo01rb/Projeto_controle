import 'package:charts_flutter/flutter.dart' as charts;
import 'package:flutter/material.dart';

class ChartRPM extends StatelessWidget {
  final List<charts.Series<dynamic, int>> seriesList;

  const ChartRPM(this.seriesList, {Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return charts.LineChart(seriesList);
  }
}

class Chart {
  final int time;
  final int rpm;

  Chart(this.time, this.rpm);
}
