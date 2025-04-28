import pygame
import sys
import serial
import threading    

# Inicialização do pygame
pygame.init()

READY = pygame.USEREVENT + 1
can_interact = True

# Tela
SCREEN_WIDTH = 800
SCREEN_HEIGHT = 600
screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
pygame.display.set_caption("Jogo da InterpreTEAção")

# Cores
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)
GREEN = (0, 200, 0)
RED = (200, 0, 0)
GRAY = (200, 200, 200)

# Fonte
font = pygame.font.SysFont(None, 48)
small_font = pygame.font.SysFont(None, 36)

# Estados do jogo
MENU = "menu"
QUIZ = "quiz"
RESULT = "result"

state = MENU
selected_answer = None
correct_answer = 1  # Resposta correta (índice)

# Carregar imagem
image = pygame.image.load("img_t_1.jpeg")
image = pygame.transform.scale(image, (300, 300))

# Função para desenhar texto
def draw_text(text, font, color, surface, x, y):
    textobj = font.render(text, True, color)
    textrect = textobj.get_rect(center=(x, y))
    surface.blit(textobj, textrect)

# Botão
def draw_button(text, x, y, width, height, inactive_color, active_color, action=None):
    mouse = pygame.mouse.get_pos()
    click = pygame.mouse.get_pressed()

    if x < mouse[0] < x + width and y < mouse[1] < y + height:
        pygame.draw.rect(screen, active_color, (x, y, width, height))
        if click[0] == 1 and action is not None and can_interact:
            action()
    else:
        pygame.draw.rect(screen, inactive_color, (x, y, width, height))

    draw_text(text, small_font, BLACK, screen, x + (width // 2), y + (height // 2))

# Transições
def start_quiz():
    global state, answer_shown, can_interact
    answer_shown = False
    can_interact = False
    pygame.time.set_timer(READY, 600)  # Espera 600ms antes de permitir interação
    state = QUIZ


def show_result(answer):
    global state, selected_answer, answer_shown, can_interact
    if not answer_shown:
        selected_answer = answer
        state = RESULT
        answer_shown = True
        can_interact = False
        pygame.time.set_timer(READY, 600)  # Espera 600ms antes de permitir interação

# === Serial ===
ser = None
def serial_thread():
    try:
        global ser
        ser = serial.Serial("COM5", baudrate=115200)
        while True:
            if ser.in_waiting>0:
                line = ser.readline().decode('utf-8', errors="ignore").strip()
                if line.startswith("BOTAO:"):
                    button = int(line.split(":")[1])
                    if state == QUIZ and button in [1, 2, 3, 4]:
                        show_result(button - 1)  # botão 1 → opção 0 ("A")
    except Exception as e:
        print("Erro na serial:", e)

# Iniciar thread serial
threading.Thread(target=serial_thread, daemon=True).start()

# Loop principal
running = True
clock = pygame.time.Clock()

while running:
    screen.fill(WHITE)

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        if event.type == READY:
            can_interact = True
            pygame.time.set_timer(READY, 0)  # Desliga o timer


    if state == MENU:
        draw_text("Jogo da InterpreTEAção", font, BLACK, screen, SCREEN_WIDTH // 2, SCREEN_HEIGHT // 3)
        draw_button("Iniciar", SCREEN_WIDTH // 2 - 75, SCREEN_HEIGHT // 2, 150, 50, GRAY, GREEN, start_quiz)

    elif state == QUIZ:
        screen.blit(image, ((SCREEN_WIDTH - 300) // 2, 40))
        options = ["Alegria", "Tristeza", "Raiva", "Felicidade"]
        for i, option in enumerate(options):
            draw_button(option, 200, 380 + i * 40, 400, 35, GRAY, GREEN, lambda i=i: show_result(i))

    elif state == RESULT:
        if selected_answer == correct_answer:
            ser.write(b'LedVermelho=0\n')
            ser.write(b'LedVerde=1\n')
            draw_text("Correto! Parabéns!", font, GREEN, screen, SCREEN_WIDTH // 2, SCREEN_HEIGHT // 2)
        else:
            ser.write(b'LedVerde=0\n')
            ser.write(b'LedVermelho=1\n')
            letras = ["A", "B", "C", "D"]
            resposta_selecionada = f"{letras[selected_answer]}: {list(enumerate(options))[selected_answer][1]}"
            resposta_correta = f"{letras[correct_answer]}: {list(enumerate(options))[correct_answer][1]}" # A: Alegria
            draw_text(f"Errado! Resposta correta {resposta_correta}", font, RED, screen, SCREEN_WIDTH // 2, SCREEN_HEIGHT // 2)
            draw_text(f"Sua resposta foi {resposta_selecionada}", font, RED, screen, SCREEN_WIDTH // 2, 50 + SCREEN_HEIGHT // 2)

        draw_button("Voltar ao Menu", SCREEN_WIDTH // 2 - 150, SCREEN_HEIGHT // 2 + 100, 300, 50, GRAY, GREEN, start_quiz)

    pygame.display.update()
    clock.tick(60)

ser.close()
pygame.quit()
sys.exit()
