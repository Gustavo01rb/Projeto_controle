import 'package:flutter/material.dart';


class WaitPage extends StatefulWidget {
  const WaitPage({super.key});

  @override
  State<WaitPage> createState() => _WaitPageState();
}

class _WaitPageState extends State<WaitPage> {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: SafeArea(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          crossAxisAlignment: CrossAxisAlignment.center,
          children:const[
            SizedBox(width: double.infinity,),
            CircularProgressIndicator(),
            Padding(
              padding: EdgeInsets.only(top:20),
              child: Text("Aguardando conexão com o servidor"),
            )
          ],
          ),
      ),
    );
  }
}