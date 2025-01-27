// ignore_for_file: prefer_const_literals_to_create_immutables, prefer_const_constructors


import 'package:crow/main.dart';
import 'package:flutter/material.dart';
import 'package:http/http.dart' as http;
import 'dart:convert';
import 'package:shared_preferences/shared_preferences.dart';
typedef JsonList = List<Map<String, dynamic>>;

class PaginaEstoque extends StatefulWidget {
  const PaginaEstoque({super.key});

  @override
  State<PaginaEstoque> createState() => _PaginaEstoqueState();
}



class _PaginaEstoqueState extends State<PaginaEstoque> {
  
  Future<JsonList>? futureData;
  @override
  void initState() { 
    super.initState();  
    futureData = receberDados();
  }

  void _atualizarFuture(valorFuture){
    futureData = valorFuture;
    selectedIds_.fillRange(0, selectedIds_.length, false);
    selectedIds.clear();
    setState(() {});
  }

void _editItemDialog(BuildContext context, int itemID, String itemNome, int itemQuantidade, String itemCategoria, String itemDescricao){
    TextEditingController nomeController = TextEditingController();
    TextEditingController categoriaController = TextEditingController();
    TextEditingController quantidadeController = TextEditingController();

    nomeController.text = itemNome;
    quantidadeController.text = itemQuantidade.toString();
    categoriaController.text = itemCategoria;


    showDialog(
      context: context,
      builder: (BuildContext context) {
        return AlertDialog(
          title: Text("Editar item"),
          content: Column(
            mainAxisSize: MainAxisSize.min,
            children: [
              TextField(
                controller: nomeController,
                decoration: InputDecoration(labelText: "Nome"),
              ),
              TextField(
                controller: categoriaController,
                decoration: InputDecoration(labelText: "Categoria"),
              ),
              TextField(
                controller: quantidadeController,
                decoration: InputDecoration(labelText: "Quantidade"),
                keyboardType: TextInputType.number,
              ),
            ],
          ),
          actions: [
            TextButton(onPressed: (){
              setState(() {
                try {
                  int.parse(quantidadeController.text);
                } catch (e) {
                    quantidadeController.text = "1";
                }

                try{
                  editarDados(itemID, nomeController.text, int.parse(quantidadeController.text), categoriaController.text);  
                } catch (e) {
                  print("Erro no editarDadosAPI: $e");
                }
                Navigator.pop(context);
              });
            }, child: Text("Editar")),
            TextButton(onPressed: (){Navigator.pop(context);}, child: Text("Cancelar"))
          ],
        );
    },);
  }

  void _longPressDialog(BuildContext context, int itemID, String itemNome, int itemQuantidade, String itemCategoria, String itemDescricao){
    showDialog(
      context: context,
      builder: (BuildContext context) {
        return AlertDialog(
          content:Column(
            mainAxisSize: MainAxisSize.min,
            
            children: [
              TextButton(onPressed: (){Navigator.pop(context); _editItemDialog(context, itemID, itemNome, itemQuantidade, itemCategoria, itemDescricao);}, child: Text("Editar")),
              TextButton(onPressed: (){
                Navigator.pop(context);
                apagarDados(itemID);}, child: Text("Apagar")),
              TextButton(onPressed: (){Navigator.pop(context);}, child: Text("Cancelar"))
            ],
          )
        );
      },
    );
  }
  void _novoItemDialog(BuildContext context){
    final TextEditingController nomeController = TextEditingController();
    final TextEditingController categoriaController = TextEditingController();
    final TextEditingController quantidadeController = TextEditingController();
    showDialog(
      context: context,
      builder: (BuildContext context) {
        return AlertDialog(
          title: Text("Novo item"),
          content: Column(
            mainAxisSize: MainAxisSize.min,
            children: [
              TextField(
                controller: nomeController,
                decoration: InputDecoration(labelText: "Nome"),
              ),
              TextField(
                controller: categoriaController,
                decoration: InputDecoration(labelText: "Categoria"),
              ),
              TextField(
                controller: quantidadeController,
                decoration: InputDecoration(labelText: "Quantidade"),
                keyboardType: TextInputType.number,
              ),
            ],
          ),
          actions: [
            TextButton(onPressed: (){
              setState(() {
                try {
                  int.parse(quantidadeController.text);
                } catch (e) {
                    quantidadeController.text = "1";
                }

                try{
                  enviarDados(nomeController.text, int.parse(quantidadeController.text), categoriaController.text);  
                } catch (e) {
                  print("Erro: $e");
                }
                Navigator.pop(context);
              });
            }, child: Text("Adicionar")),
            TextButton(onPressed: (){Navigator.pop(context);}, child: Text("Cancelar"))
          ],
        );
    },);
  }

  

Future<List<Map<String,dynamic>>> receberDados() async {

  final url = Uri.parse('http://10.0.0.200:18080/produtos/${await getUser()}');
  try {
    // Aguarde o retorno do getToken antes de passar para o cabeçalho
    final token_ = await getToken();  // Use await aqui para obter o valor da String
    final response = await http.get(
      url,
      headers: {'Authorization': token_}, // Aplique o token corretamente no header
    );

    if (response.statusCode == 200) {
      final List<dynamic> itemList = json.decode(response.body)["corpo"];
      return itemList.map((e) => e as Map<String, dynamic>).toList();
    } else {
      print("Erro na requisição: ${response.statusCode}");
      return [];
    }
  } catch (e) {
    print("Erro: $e");
    return [];
  }
}

Future<List<Map<String,dynamic>>> pesquisarDados(nome) async {
  final url = Uri.parse('http://10.0.0.200:18080/produtos/${await getUser()}/${nome}');
  try{
    final response = await http.get(
      url,
      headers: {"Authorization": await getToken()}
      );
    if (response.statusCode == 200) {
      final List<dynamic> itemList = json.decode(response.body)["corpo"];
      return itemList.map((e) => e as Map<String, dynamic>).toList();
    } else {
      print("Erro na requisição: ${response.statusCode}");
      return [];
    }
    } catch (e) {
      print("Erro: $e");
      return [];
    }
}


Future<void> enviarDados(nome, quantidade, categoria) async {
  final url = Uri.parse('http://10.0.0.200:18080/adicionar-produto');
  final userid_ = await getUser();
  final Map<String, dynamic> dados = {'nome': nome, 'quantidade': quantidade, 'categoria': categoria, 'userid': userid_};

  try {
    final response = await http.post(
      url,
      headers: {
        "Content-Type": "application/json",
        "Authorization": await getToken()
      },
      body: json.encode(dados),
    ).catchError((error) {
      print("Erro ao enviar requisição: $error");
    });
    if (response.statusCode == 200) {
      print('Requisição bem-sucedida: ${response.body}');
      _atualizarFuture(receberDados());
    } else {
      print('Falha na requisição: ${response.statusCode}');
    }
  } catch (e) {
    print("Erro: $e");
  }
}

Future<void> apagarDados(id) async {

  final url = Uri.parse('http://10.0.0.200:18080/produtos/$id');

  String token_ = await getToken();
  String userid_ = await getUser();

  final Map<String, dynamic> dados = {
    'userid': userid_
  };

  final response = await http.delete(
    url,
    headers: {"Content-Type": "application/json", "Authorization": token_},
    body: json.encode(dados),
  );

  // Verificar a resposta do servidor
  if (response.statusCode == 200) {
    print('Requisição bem-sucedida: ${response.body}');
    _atualizarFuture(receberDados());
  } else {
    print('Falha na requisição: ${response.statusCode} ${response.body}');
  }
}



Future<void> editarDados(id, nome, quantidade, categoria) async {
  final url = Uri.parse("http://10.0.0.200:18080/produtos");
  final Map<String,dynamic> dados = {
    'id': id,
    'nome': nome,
    'quantidade': quantidade,
    'categoria': categoria,
    'userid': await getUser()
    };

  try {
    final response = await http.put(
      url,
      headers: {"Content-Type" : "application/json", "Authorization": await getToken()},
      body: json.encode(dados)
    ). catchError((error) {
      print("Erro ao enviar requisição: $error");
    });

    if (response.statusCode == 200) {
      print('Requisição bem-sucedida: ${response.body}');
      _atualizarFuture(receberDados());
    } else {
      print('Falha na requisição: ${response.statusCode}');
    }
  } catch (e) {
    print("Erro: $e");
  }
}




  final double tamanhoFonte = 14;
  final double tamanhoBox = 24;
  List<int> selectedIds = [];
  List<bool> selectedIds_ = [];

  @override
  Widget build(BuildContext context) {
    return Column(
      crossAxisAlignment: CrossAxisAlignment.center,
      children: [
        Row(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            Text("Estoque", style: TextStyle(fontWeight: FontWeight.w600, fontSize: 20)),
            IconButton(onPressed: (){setState(() {_atualizarFuture(receberDados());});}, icon: Icon(Icons.refresh))
          ],
        ),
        SizedBox(height: 20,),
        Row(
          children: [
            SizedBox(width: 50,),
            // Padding(
            //   padding: const EdgeInsets.all(8.0),
            //   child: ElevatedButton.icon(
            //     onPressed: (){},
            //     style: ButtonStyle(
            //       side: MaterialStatePropertyAll(BorderSide(width: 1)),
                  
                  
            //       ),
            //     icon: Icon(Icons.sort, color: Colors.black),
            //     label: Text("Filtrar", style: TextStyle(color: Colors.black)), 
            //     ),
            // ),
            Expanded(
              flex:5,
              child: SearchBar(
                leading: Icon(Icons.search),
                onSubmitted: (value) {
                  _atualizarFuture(pesquisarDados(value));
                },
              ),
            ),
            Padding(
              padding: const EdgeInsets.all(8.0),
              child: ElevatedButton.icon(
                onPressed: () {_novoItemDialog(context);},
                style: ButtonStyle(side: MaterialStatePropertyAll(BorderSide(width: 1))),
                icon: Icon(Icons.add, color: Colors.black),
                label: Text("Novo", style: TextStyle(color: Colors.black)),
                ),
            )
        ]),
        SizedBox(height: 20),
        Expanded(
          child: FutureBuilder<JsonList>(future: futureData, builder: (context, snapshot){
            if (snapshot.connectionState == ConnectionState.waiting) {
              return Center(child: CircularProgressIndicator());
            } else if (snapshot.hasError){
              return Center(
                child: Text("Não foi possível carregar a tabela. Erro: ${snapshot.error}"),
              );
            } 
          
            final data = snapshot.data!;
            
            if(data.isEmpty){
              return Text("Não foi possível carregar a tabela ou ela está vazia.");
            }

            if (selectedIds_.length != data.length) {
              if (selectedIds_.length < data.length) {
                // Adicionar valores `false` para novos itens
                selectedIds_.addAll(List<bool>.filled(data.length - selectedIds_.length, false));
              } else if (selectedIds_.length > data.length) {
                // Reduzir o tamanho da lista para corresponder ao tamanho dos dados
                selectedIds_ = selectedIds_.sublist(0, data.length);
              }
            }
            
            return SingleChildScrollView(
              scrollDirection: Axis.horizontal,
              child: SingleChildScrollView(
                scrollDirection: Axis.vertical,
                child: DataTable(
                  
                  columns: [
                  DataColumn(label: Text("Item", overflow: TextOverflow.ellipsis,)),
                  DataColumn(label: Text("Categoria")),
                  DataColumn(label: Text("Descrição")),
                  DataColumn(label: Text("Quantidade")),
                  ],
                  rows: List.generate(data.length, (index) {
                    final item = data[index];
                     return DataRow(
                      
                      onLongPress: () {
                        _longPressDialog(context, item['id'], item['nome'], item['quantidade'], item['categoria'], "N/A");
                      },
                      
                      cells: [
                        DataCell(Text(item['nome'])),
                        DataCell(Text(item['categoria'])),
                        DataCell(Text("N/A")),
                        DataCell(Text(item['quantidade'].toString())),
                      ]
                    );
                  }),
                ),
              ),
            );
          }),
        ),
    ]);
  }
}