import pygame, random, math, numpy                  

class App:
    size = 16
    bombs = 32
    screenheight = 700
    screenwidth = 600
    cooldown = 0
    gamestate = "playing"
    running = True
    adjacents = numpy.array([[-1,-1],[0,-1],[1,-1],[-1,0],[1,0],[-1,1],[0,1],[1,1]])

    icons = {0:pygame.image.load("0.png"),
         1:pygame.image.load("1.png"),
         2:pygame.image.load("2.png"),
         3:pygame.image.load("3.png"),
         4:pygame.image.load("4.png"),
         5:pygame.image.load("5.png"),
         6:pygame.image.load("6.png"),
         7:pygame.image.load("7.png"),
         8:pygame.image.load("8.png"),
         -1:pygame.image.load("bomb.png"),
         "f":pygame.image.load("flag.png"),
         "gg":pygame.image.load("gameOver.png")}

    def __init__(self):

        # Initialise pygame and window
        pygame.init()
        self.win = pygame.display.set_mode((self.screenwidth, self.screenheight))
        self.clock = pygame.time.Clock()
        
        self.generate()
        self.game_loop()

    def generate(self):

        self.gamestate = "playing"

        # Create two 2D arrays (size x size) 
        self.map = numpy.zeros([self.size, self.size])
        self.shown = numpy.zeros([self.size, self.size])

        # Create box sizes
        self.boxlength = self.screenwidth/(self.size*4+1)
        self.boxsize = math.floor(self.boxlength*3)

        # Generate bombs
        for i in range(self.bombs):
            self.map[random.randint(0,self.size-1)][random.randint(0,self.size-1)] = -1

        # Get the number of adjacent bombs for each tile
        for i in range(self.size):
            for j in range(self.size):

                # If tile is not a bomb
                if self.map[i][j] != -1:

                    # Get adjacent bombs 
                    for n in self.adjacents:
                        try:
                            if self.map[i+n[0]][j+n[1]] == -1 and -1<i+n[0] and -1<j+n[1]:
                                self.map[i][j]+=1
                        except: pass

    def game_loop(self):
        while self.running:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    self.running = False
            
            if self.gamestate == "playing":
                self.win.fill((80,80,80))
                self.mouseupdate()
                self.display()
                self.cooldown-=1

            if self.gamestate == "ended":
                if pygame.key.get_pressed()[pygame.K_r]: self.generate()
                self.win.blit(pygame.transform.scale(self.icons["gg"], 
                            (self.screenwidth, math.floor(self.screenwidth / 5))),
                            (0, (self.screenheight - self.screenwidth / 5) / 2))

            pygame.display.update()
            self.clock.tick(30)
        pygame.quit()
    
    def mouseupdate(self):

        # Mouse pos to grid pos
        mousePos = pygame.mouse.get_pos()
        x = int((mousePos[0] - self.boxlength/2) // (self.boxlength * 4))
        y = int((mousePos[1] - self.boxlength/2 - 100) // (self.boxlength * 4))
        
        # Check the mouse press is in range
        if (self.size - 3 < x < 0 or self.size - 3 < y < 0): 
            return

        # Uncover tile
        if pygame.mouse.get_pressed()[0]:
            self.shown[x][y] = 1
            match self.map[x][y]:
                case -1:
                    self.gamestate = "ended"
                case 0:
                    self.flood_fill(x, y)
            

        # Place flag
        if pygame.mouse.get_pressed()[2] and self.cooldown < 0:
            if self.shown[x][y] == -1:
                self.shown[x][y]=0
            elif self.shown[x][y] == 0:
                self.shown[x][y]=-1  

            self.cooldown = 10  
    
    def display(self):
        # Topbar background
        pygame.draw.rect(self.win, (120, 120 , 120), (0, 0, self.screenwidth , 100))

        for i in range(self.size):
            for j in range(self.size):
                
                # Create Rect
                box = (math.floor(self.boxlength * i * 4 + self.boxlength), 
                       math.floor(self.boxlength * j * 4 + self.boxlength + 100), 
                       self.boxsize, self.boxsize)
                
                # Display background
                pygame.draw.rect(self.win, (120, 120 , 120), box)
                
                # Display uncovered tiles
                if self.shown[i][j]==1:
                    self.win.blit(pygame.transform.scale(self.icons[self.map[i][j]], 
                                (self.boxsize, self.boxsize)), box)
                    
                # Display flags
                elif self.shown[i][j]==-1:
                    self.win.blit(pygame.transform.scale(self.icons["f"], 
                                (self.boxsize, self.boxsize)), box)

    def flood_fill(self, x, y):
        for adj in self.adjacents:
            adjX = x + adj[0]
            adjY = y + adj[1]
            try:
                if  self.size > adjY >= 0 and self.size > adjX >= 0 and self.shown[adjX][adjY] == 0:
                    self.shown[adjX][adjY] = 1
                    if self.map[adjX][adjY] == 0:
                        self.flood_fill(adjX, adjY)
            except: pass


if __name__ == "__main__":
    App()