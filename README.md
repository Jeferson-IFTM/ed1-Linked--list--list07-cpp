# ed1-linked-list-list07-cpp

**Estrutura de Dados I** — IFTM Campus Patrocínio — TADS, 3º período (noturno)
**Atividade 7:** Algoritmos em Listas Encadeadas Simples (C++)
**Professor:** Júnio Moreira

> Aluno: _preencher nome_ — Matrícula: _preencher_

## Estrutura do projeto

```
ed1-linked-list-list07-cpp/
├── include/
│   ├── List.h              # interface abstrata List<T>
│   └── LinkedList.h        # Node<T>, LinkedList<T> e as funções das questões
├── src/
│   └── LinkedListTest.cpp  # testes automatizados (94 verificações)
├── main.cpp                # demonstração com os exemplos do enunciado
├── CMakeLists.txt
├── .gitignore
└── README.md
```

A pasta `cmake-build-debug/` é gerada pela IDE (CLion) e está no `.gitignore`.

## Requisitos

- Compilador com suporte a **C++17** (g++ ≥ 8, clang ≥ 7 ou MSVC 2019+)
- **CMake ≥ 3.14** (opcional; há alternativa com g++ direto)

## Compilação e execução

### Opção 1 — CMake (Linux / macOS / Windows com MinGW)

```bash
git clone <URL-DO-REPOSITORIO>
cd ed1-linked-list-list07-cpp

cmake -S . -B build
cmake --build build

./build/list07          # demonstração dos exemplos do enunciado
./build/list07_tests    # testes automatizados
```

No Windows (PowerShell, com MinGW): `.\build\list07.exe` e `.\build\list07_tests.exe`.
Com Visual Studio, os executáveis ficam em `build\Debug\`.

Para rodar via CTest: `ctest --test-dir build --output-on-failure`.

### Opção 2 — g++ direto (sem CMake)

```bash
g++ -std=c++17 -Wall -Wextra -Iinclude main.cpp -o list07
g++ -std=c++17 -Wall -Wextra -Iinclude src/LinkedListTest.cpp -o list07_tests

./list07
./list07_tests
```

### Opção 3 — CLion

Abra a pasta do projeto (`File > Open`), selecione o alvo `list07` ou `list07_tests` e clique em **Run**.

### Verificação de memória (opcional)

```bash
cmake -S . -B build-asan -DENABLE_SANITIZERS=ON
cmake --build build-asan
./build-asan/list07_tests
```

Habilita AddressSanitizer + UBSan para detectar *memory leaks*, *use-after-free* e comportamento indefinido.
Saída esperada dos testes: `TODOS OS TESTES PASSARAM.`

## Resumo das soluções

| # | Função | Abordagem | Tempo | Espaço |
|---|--------|-----------|-------|--------|
| 1 | `addSorted(item)` | Trata lista vazia, início, fim (via `tail`) e meio (com predecessor); atualiza `head`, `tail` e `count` | O(N) | O(1) |
| 2 | `deleteNodeWithoutHead(P)` | *Node copy*: copia o sucessor para `P`, religa `next` e dá `delete` no sucessor | O(1) | O(1) |
| 3 | `reverse()` | Três ponteiros (`prev`, `cur`, `next`); troca `head` e `tail` | O(N) | O(1) |
| 4 | `mergeTwoLists(l1, l2)` | Religa os `next` dos nós existentes escolhendo sempre o menor; estável | O(N+M) | O(1) |
| 5 | `removeNthFromEnd(head, n)` | `fast` avança `n` nós; depois `fast`/`slow` andam juntos em uma passagem | O(N) | O(1) |

### Questão 1 — `addSorted`

O código original tinha dois defeitos: `current->next = newNode` desreferencia `nullptr` quando a inserção é no fim ou em lista vazia (*segmentation fault*), e o percurso para no nó errado, pois sem predecessor não é possível religar o nó anterior (e o laço ainda usa a comparação invertida para ordem crescente). A versão corrigida separa os quatro casos: lista vazia, novo menor que `head`, novo maior ou igual a `tail` (inserção em O(1)) e meio, percorrendo com o predecessor.

### Questão 2 — `deleteNodeWithoutHead`

Sem acesso ao predecessor, não é possível fazer o nó anterior "pular" `P`. A solução é remover o **sucessor** de `P` depois de copiar o conteúdo dele para `P`:

```
20 -> 4 -> [5] -> 19 -> 47
1) P->item = P->next->item      // P passa a guardar 19
2) P->next = P->next->next      // P pula o antigo sucessor
3) delete (antigo sucessor)     // libera memória, sem leak
20 -> 4 -> 19 -> 47
```

**Por que não funciona se `P` é o último nó:** `P->next == nullptr`, então não há sucessor para copiar. O nó anterior continuaria apontando para `P`, e sem acesso a ele não há como retirá-lo da cadeia. A função retorna `false` nesse caso. Outra limitação: o conteúdo dos nós é trocado de lugar, então ponteiros externos para o sucessor ficam inválidos.

A versão membro `LinkedList::deleteNodeWithoutHead` também mantém `tail` e `count` consistentes.

### Questão 3 — `reverse`

Percorre a lista uma vez, invertendo cada `next` para apontar ao predecessor. Ao final, `head` recebe o antigo último nó e `tail` recebe o antigo primeiro. Nenhum nó é alocado.

### Questão 4 — `mergeTwoLists`

Define a cabeça pelo menor primeiro elemento e anexa em seguida o menor entre as frentes das duas listas. Quando uma lista acaba, o restante da outra (já ordenado) é anexado de uma vez. Em empate, o nó de `list1` vem primeiro (estável). Nenhum nó é alocado. A classe também oferece `LinkedList::mergeSorted(other)`.

### Questão 5 — `removeNthFromEnd`

1. `fast` avança `n` posições a partir de `head`.
2. Se `fast` virou `nullptr`, `n` é igual ao tamanho: remove-se o próprio `head`.
3. Caso contrário, `fast` e `slow` avançam juntos até `fast->next == nullptr`; então `slow->next` é o n-ésimo nó do fim.
4. `slow->next` é desligado e liberado com `delete`.

Não usa `count`/`size` nem pré-contagem. Para `n <= 0` ou `n` maior que o tamanho, a lista é devolvida sem alteração.

## Entrega

1. Criar um repositório **público** no GitHub (ou privado com acesso ao professor) chamado `ed1-linked-list-list07-cpp`.
2. Enviar o código:

   ```bash
   git init
   git add .
   git commit -m "Lista 07: algoritmos em listas encadeadas simples"
   git branch -M main
   git remote add origin https://github.com/<usuario>/ed1-linked-list-list07-cpp.git
   git push -u origin main
   ```

3. Anexar o **link direto do repositório** no campo de envio do Moodle.
4. Segunda etapa: enviar o **vídeo demonstrativo** da aplicação, conforme orientação no Moodle.
