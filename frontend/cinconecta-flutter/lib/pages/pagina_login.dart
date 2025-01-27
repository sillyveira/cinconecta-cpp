import 'package:flutter/material.dart';
import 'package:http/http.dart' as http;
import 'dart:convert';
import 'package:shared_preferences/shared_preferences.dart';

SnackBar snack(String texto) {
  return SnackBar(
    content: Text(texto),
    showCloseIcon: true,
    duration: Durations.medium4,
  );
}

InputDecoration customInputDecoration(String hintText) {
  return InputDecoration(
    hintText: hintText,
    hintStyle:
        TextStyle(color: Colors.white54), // Cor da dica do campo (placeholder)
    filled: true,
    fillColor: Colors.grey[800], // Cinza médio-escuro
    contentPadding: EdgeInsets.symmetric(horizontal: 20, vertical: 16),
    border: OutlineInputBorder(
      borderRadius: BorderRadius.circular(30), // Bordas arredondadas
      borderSide: BorderSide.none, // Sem bordas visíveis
    ),
    focusedBorder: OutlineInputBorder(
      borderRadius: BorderRadius.circular(30),
      borderSide: BorderSide(color: Colors.white, width: 2), // Borda ao focar
    ),
    enabledBorder: OutlineInputBorder(
      borderRadius: BorderRadius.circular(30),
      borderSide: BorderSide(color: Colors.grey[700]!), // Borda sutil
    ), // Ícone opcional
  );
}

Future<Map<String, dynamic>> loginAPI(nomeUsuario, senha, context, Function(bool) onLogin, setUser) async {
  final url = Uri.parse('http://10.0.0.200:18080/login');
  try {
    final response = await http.post(url,
        body: jsonEncode({"usuario": nomeUsuario, "senha": senha}));
    dynamic jsonBody = jsonDecode(response.body);

    if (response.statusCode == 200) {
      ScaffoldMessenger.of(context)
          .showSnackBar(snack("O login foi feito com sucesso"));
      try {
        final SharedPreferences prefs = await SharedPreferences.getInstance();
        prefs.setString('token', jsonBody["dados"]["token"]);
        prefs.setString('userid', jsonBody["dados"]["userid"]);
        prefs.setString('username', nomeUsuario);
        onLogin(true);
        setUser(nomeUsuario);

      } catch (e) {
        ScaffoldMessenger.of(context)
            .showSnackBar(snack("Erro. Não foi possível realizar o login."));
      }
      print(jsonDecode(response.body));
      return {};
    } else {
      print("Erro na requisição: ${response.statusCode} ${response.body}");
      ScaffoldMessenger.of(context).showSnackBar(snack(jsonBody["mensagem"]));
      return {};
    }
  } catch (e) {
    print("Erro: $e");
    return {};
  }
}

Future<Map<String, dynamic>> registroAPI(nomeUsuario, senha, context) async {
  final url = Uri.parse('http://10.0.0.200:18080/registrar');
  try {
    final response = await http.post(url,
        body: jsonEncode({"usuario": nomeUsuario, "senha": senha}));
    if (response.statusCode == 200) {
      ScaffoldMessenger.of(context)
          .showSnackBar(snack("O registro foi feito com sucesso"));
      return json.decode(response.body);
    } else {
      print("Erro na requisição: ${response.statusCode} ${response.body}");
      ScaffoldMessenger.of(context)
          .showSnackBar(snack(json.decode(response.body)["mensagem"]));
      return {};
    }
  } catch (e) {
    print("Erro: $e");
    return {};
  }
}

Future<Map<String, dynamic>> teste(userid, token) async {
  final url = Uri.parse('http://10.0.0.1:18080/login');
  try {
    final response = await http
        .post(url, headers: {"Authorization": token}, body: {"userid": userid});
    if (response.statusCode == 200) {
      return json.decode(response.body);
    } else {
      print("Erro na requisição: ${response.statusCode}");
      return {};
    }
  } catch (e) {
    print("Erro: $e");
    return {};
  }
}

class LoginPage extends StatefulWidget {
  Function(bool) onLogin;
  void Function(String) setUser;

  LoginPage({super.key, required this.onLogin, required this.setUser});

  @override
  State<LoginPage> createState() => _LoginPageState();
}

class _LoginPageState extends State<LoginPage> {
  TextEditingController nomeUsuarioController = TextEditingController();
  TextEditingController senhaController = TextEditingController();

  @override
  Widget build(BuildContext context) {
    return Center(
      child: SizedBox(
        height: 450,
        width: 320,
        child: Container(
          padding: EdgeInsets.all(24), // Espaçamento interno
          decoration: BoxDecoration(
            borderRadius: BorderRadius.circular(30), // Bordas mais suaves
            color: Colors.grey[900], // Cor de fundo mais escura para contraste
            boxShadow: [
              BoxShadow(
                color: Colors.black26,
                blurRadius: 10,
                spreadRadius: 2,
                offset: Offset(0, 4), // Sombra para efeito elevado
              ),
            ],
          ),
          child: Column(
            mainAxisAlignment: MainAxisAlignment.spaceEvenly,
            children: [
              Text(
                "Acesse sua conta",
                style: TextStyle(
                  fontSize: 22,
                  fontWeight: FontWeight.bold,
                  color: Colors.white,
                ),
              ),
              Column(
                children: [
                  TextField(
                    decoration: customInputDecoration("Usuário"),
                    controller: nomeUsuarioController,
                    textAlign: TextAlign.center,
                    style: TextStyle(color: Colors.white), // Texto branco
                  ),
                  SizedBox(height: 15),
                  TextField(
                    decoration: customInputDecoration("Senha"),
                    controller: senhaController,
                    textAlign: TextAlign.center,
                    obscureText: true, // Esconder senha
                    style: TextStyle(color: Colors.white),
                  ),
                ],
              ),
              Column(
                children: [
                  ElevatedButton(
                    onPressed: () {
                      loginAPI(nomeUsuarioController.text, senhaController.text,
                          context, widget.onLogin, widget.setUser);
                    },
                    style: ElevatedButton.styleFrom(
                      backgroundColor: Colors.blueAccent, // Cor do botão
                      shape: RoundedRectangleBorder(
                        borderRadius:
                            BorderRadius.circular(30), // Botão arredondado
                      ),
                      padding:
                          EdgeInsets.symmetric(vertical: 15, horizontal: 50),
                    ),
                    child: Text(
                      "Login",
                      style: TextStyle(fontSize: 18, color: Colors.white),
                    ),
                  ),
                  SizedBox(height: 10),
                  TextButton(
                    onPressed: () {
                      registroAPI(nomeUsuarioController.text,
                          senhaController.text, context);
                    },
                    child: Text(
                      "Registrar",
                      style: TextStyle(
                        fontSize: 16,
                        color: Colors.blueAccent,
                        fontWeight: FontWeight.w600,
                      ),
                    ),
                  ),
                ],
              ),
              
            ],
            
          ),
        ),
      ),
    );
  }
}
