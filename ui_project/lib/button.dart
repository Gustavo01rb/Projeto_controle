import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';

class ButtonDefault extends StatefulWidget {
  const ButtonDefault({ Key? key, this.width, required this.text, required this.onPressed, this.color }) : super(key: key);

  final String         text;
  final VoidCallback? onPressed;
  final Color?         color;
  final double?        width; 


  @override
  // ignore: library_private_types_in_public_api
  _ButtonDefaultState createState() => _ButtonDefaultState();
}

class _ButtonDefaultState extends State<ButtonDefault> {
  @override
  Widget build(BuildContext context) {
    final double width = MediaQuery.of(context).size.width;
    return SizedBox(
      width: widget.width ?? width,
      child: CupertinoButton(
        onPressed: widget.onPressed,
        color: widget.color ?? Theme.of(context).colorScheme.tertiary,
        child:  Text(widget.text) ,
        
      ),
    );
  }
}