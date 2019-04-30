library(dbscan)
name <- "Mosmann_rare.fcs"
path <- "C:\\Users\\User1\\Downloads\\DATA\\Labeled\\"
fcs_read_mosmann_rare = read.FCS(paste(path, name, sep=""));
fcs_exprs = fcs_read@exprs
opt_fcs = optics(fcs_read_mosmann_rare, eps = 30, minPts = 100)
plot(opt_fcs)
