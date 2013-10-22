stats <- read.table("out") 


humans = stats$V1
infected = stats$V2
zombies = stats$V3
empty = stats$V4
total = stats$V5

postscript("zombies.ps")
plot(zombies)
dev.off()

postscript("humans.ps")
plot(humans)
dev.off()

postscript("infected.ps")
plot(infected)
dev.off()
