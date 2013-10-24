stats <- read.table("out") 


humans = stats$V1
infected = stats$V2
zombies = stats$V3
empty = stats$V4
total = stats$V5

humanZombie = rbind(humans, zombies)


postscript("zombies.ps")
    plot(zombies)
dev.off()

postscript("humans.ps")
    plot(humans)
dev.off()

postscript("humansZombies.ps")
    matplot(t(humanZombie))
dev.off()

postscript("infected.ps")
    plot(infected)
dev.off()


