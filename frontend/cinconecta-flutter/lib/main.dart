// ignore_for_file: prefer_const_constructors, prefer_const_literals_to_create_immutables

import 'package:crow/pages/pagina_estoque.dart';
import 'package:crow/pages/pagina_inicial.dart';
import 'package:crow/pages/pagina_login.dart';
import 'package:flutter/material.dart';
import 'package:http/http.dart' as http;
import 'package:shared_preferences/shared_preferences.dart';
import 'dart:convert';

void main() {
  runApp(MaterialApp(home: MyApp(),));
}

Future<String> getToken() async {
  final SharedPreferences prefs = await SharedPreferences.getInstance();
  return prefs.getString('token') ?? '';
}

Future<String> getUser() async {
  final SharedPreferences prefs = await SharedPreferences.getInstance();
  return prefs.getString('userid') ?? '';
}

Future<void> deslogarAPI(token, context) async {
  final url = Uri.parse('http://10.0.0.200:18080/deslogar');
  try {
    final response = await http.post(url,
        body: jsonEncode({"token": token}));
    dynamic jsonBody = jsonDecode(response.body);

    if (response.statusCode == 200) {
      ScaffoldMessenger.of(context)
          .showSnackBar(snack("Você foi deslogado com sucesso."));
      try {
        final SharedPreferences prefs = await SharedPreferences.getInstance();  
        prefs.remove('token');
        prefs.remove('userid');
        
      } catch (e) {
      // se não conseguir remover as keys no cliente, tudo bem, elas foram removidas no servidor.   
      }
      print(jsonDecode(response.body));

    } else {
      print("Erro na requisição: ${response.statusCode} ${response.body}");
      ScaffoldMessenger.of(context).showSnackBar(snack(jsonBody["mensagem"]));
    }
  } catch (e) {
    print("Erro: $e");
  }
}



class MyApp extends StatefulWidget {
  MyApp({super.key});

  @override
  State<MyApp> createState() => _MyAppState();

  
}

class _MyAppState extends State<MyApp> {

  

  int telaIndex_ = 0;
  bool mostrarDrawer = false;
  String userName = "N/A";

  void setMostrarDrawer(bool valor){
    setState(() {
      mostrarDrawer = valor;
      telaIndex_ = 1;
    });
  }

  void setUser(String name){
      setState(() {
        userName = name;
      });
    }

  final List<dynamic> telasNavigation = [
      LoginPage(onLogin: (valor){}, setUser: (valor){},),
      PaginaInicial(),
      PaginaEstoque()
    ];
  @override
  Widget build(BuildContext context) {
    telasNavigation[0] = LoginPage(onLogin: setMostrarDrawer, setUser: setUser);
  return Scaffold(
    body: telasNavigation[telaIndex_],
    drawer: mostrarDrawer 
        ? Drawer(
            child: Column(
              children: [
                Expanded(
                  child: ListView(
                    padding: EdgeInsets.all(0),
                    children: [
                      DrawerHeader(
                        child: Column(children: [
                          CircleAvatar(minRadius: 50, maxRadius: 50),
                          SizedBox(height: 10),
                          Text(userName)
                        ]),
                      ),
                      ListTile(
                        title: Text("página inicial"),
                        leading: Icon(Icons.home),
                        onTap: () {
                          Navigator.pop(context);
                          setState(() {
                            telaIndex_ = 1;
                          });
                        },
                      ),
                      // ListTile(
                      //   title: Text("análise geral"),
                      //   leading: Icon(Icons.analytics),
                      //   onTap: () {
                      //     Navigator.pop(context);
                      //     setState(() {
                      //       telaIndex_ = 2;
                      //     });
                      //   },
                      // ),
                      ListTile(
                        title: Text("estoque"),
                        leading: Icon(Icons.remove_red_eye),
                        onTap: () {
                          Navigator.pop(context);
                          setState(() {
                            telaIndex_ = 2;
                          });
                        },
                      )
                    ],
                  ),
                ),
                ListTile(
                  leading: Icon(Icons.logout),
                  title: Text("Deslogar"),
                  onTap: () async {
                    final SharedPreferences prefs = await SharedPreferences.getInstance();
                    String token = prefs.getString('token') ?? '';
                    print("deslogarAPI sendo chamado em ${token}");
                    // Aguarda a função deslogarAPI ser completada
                    await deslogarAPI(token, context);
                    Navigator.of(context).pop();

                    setState(() {
                      telaIndex_ = 0;  
                    });
                    

                    // Adicione a navegação ou qualquer outra ação após o logout, se necessário
                  },
                )
              ],
            ),
          )
        : null,  // Não exibe a Drawer se mostrarDrawer for falso
    appBar: AppBar(
      title: Text("CinConecta", textAlign: TextAlign.center),
    ),
  );
  }
}
