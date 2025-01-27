import 'package:flutter/material.dart';

class PaginaInicial extends StatelessWidget {
  const PaginaInicial({super.key});

  @override
  Widget build(BuildContext context) {
    return Column(
      mainAxisAlignment: MainAxisAlignment.center,
      children: [
        Center(child: Image.asset("lib/img/tela-inicial/telainicial.png", width: 350)),
        SizedBox(height: 20,),
        Text("BEM VINDO(A), USUÁRIO!", textAlign: TextAlign.center,),
        SizedBox(height: 10,),
        Text("Nossa missão é facilitar o controle de estoque e a gestão de doações para organizações sociais em Pernambuco, permitindo que você dedique mais tempo às ações transformadoras que realizam.", textAlign: TextAlign.center,)

      ],
    );
  }
}