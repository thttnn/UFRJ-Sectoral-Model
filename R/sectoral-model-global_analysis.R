#******************************************************************
#
# ----------------- Sectoral Model Aggregate analysis ---------------
#
#******************************************************************

#******************************************************************
#
# ------------ Read Monte Carlo experiment files ----------------
#
#******************************************************************

folder    <- "./Sectoral_Model_Inflation"                  # data files folder
baseName  <- "Sim_"                     # data files base name (same as .lsd file)
nExp      <- 4                        # number of experiments (sensitivity/different cases)
iniDrop   <- 0                          # initial time steps to drop from analysis (0=none)
nKeep     <- -1                         # number of time steps to keep (-1=all)
cores     <- 0                          # maximum number of cores to allocate (0=all)
savDat    <- F                          # save processed data files and re-use if available?

expVal <- c("Baseline", "Input_Price", "Markup", "Markup 2")                   # case parameter values

# Aggregated variables to use
logVars <- c( "Sector_Wages",               
              "Sector_Profits",                  
              "Sector_Income",                   
              "Sector_Avg_Price",                   
              "Sector_Avg_Wage",                
              "Sector_Avg_Productivity",
              "Sector_Effective_Production",
              "Sector_Effective_Orders",
              "Sector_Productive_Capacity"
              )

aggrVars <- append( logVars, 
                    c(  "Sector_Capacity_Utilization",        
                        "Sector_Income_Growth",         
                        "Sector_Inflation",        
                        "Sector_Profit_Rate",       
                        "Sector_Normalized_HHI"
                        ) )

# Variables to test for stationarity and ergodicity
    statErgo.vars <- c( )

# Temporary data file suffix
datFilSfx <- "_aggr.Rdata"

# ==== Log start mark ====

cat( "\nMicro-macro aggregates analysis\n=======================\n" )
cat( "\n", as.character( Sys.time( ) ), "-> Start processing...\n\n" )
startTime <- proc.time( )       # register current time
options( warn = -1 )

# ==== Read command line parameters (if any) ====
args <- commandArgs( trailingOnly = TRUE )
cat( "Command line arguments: ", args, "\n" )

if( length ( args ) > 0 ){  # first parameter has to be the folder
  folder <- args [1]
}
if( length ( args ) > 1 ){  # second parameter has to be the base name
  baseName <- args [2]
}
if( length ( args ) > 2 ){  # third parameter has to be the number of experiments
  nExp <- as.integer( args [3] )
}
if( length ( args ) > 3 ){  # fourth parameter has to be the initial time period ( 0 is all )
  iniDrop <- as.integer( args [4] )
}
if( length ( args ) > 4 ){  # fifth parameter has to be the end periods to remove ( -1 is all )
  nKeep <- as.integer( args [5] )
}
if( length ( args ) > 5 ){  # sixth parameter has to be the number of cores ( 0 is all )
  cores <- as.integer( args [6] )
}
if( length ( args ) > 6 ){  # seventh parameter has to be the intermediate data saving flag
  savDat <- as.logical( args [7] )
}

cat( " Folder =", folder, "\n" )
cat( " Base name =", baseName, "\n" )
cat( " Number of experiments =", nExp, "\n" )
cat( " Initial time steps to drop =", iniDrop, "\n" )
cat( " Time steps to keep =", nKeep, "\n" )
cat( " Maximum cores to use =", cores, "\n" )
cat( " Re-use data files =", savDat, "\n\n" )

# ==== Process LSD result files ====
# Package with LSD interface functions
library( LSDinterface, verbose = FALSE, quietly = TRUE )
library( parallel, verbose = FALSE, quietly = TRUE )

# ==== Read data files ----

# Function to read one experiment data (to be parallelized)
readExp <- function( exper ) {
  if( nExp > 1 ) {
    myFiles <- list.files( path = folder, pattern = paste0( baseName, exper, "_[0-9]+.res" ),
                           full.names = TRUE )
  } else {
    myFiles <- list.files( path = folder, pattern = paste0( baseName, "_[0-9]+.res" ),
                           full.names = TRUE )
  }
  
  if( length( myFiles ) < 1 )
    stop( "Data files not found. Check 'folder', 'baseName' and 'nExp' parameters." )
  
  cat( "\nData files: ", myFiles, "\n" )
  
  # Read data from text files and format it as a 3D array with labels
  mc <- read.3d.lsd( myFiles, aggrVars, skip = iniDrop, nrows = nKeep, nnodes = lsdCores )
  
  # Get dimensions information
  nTsteps <- dim( mc )[ 1 ]              # number of time steps
  nVar <- dim( mc )[ 2 ]                 # number of variables
  nSize  <- dim( mc )[ 3 ]               # Monte Carlo sample size
  
  cat( "\nData details:\n" )
  cat( " Number of MC cases =", nSize, "\n" )
  cat( " Number of variables =", nVar, "\n" )
  cat( " Number of used periods =", nTsteps, "\n\n" )
  
  # Compute Monte Carlo averages and std. deviation and store in 2D arrrays
  stats <- info.stats.lsd( mc )
  
  # Insert a t column
  t <- as.integer( rownames( stats$avg ) )
  A <- as.data.frame( cbind( t, stats$avg ) )
  S <- as.data.frame( cbind( t, stats$sd ) )
  M <- as.data.frame( cbind( t, stats$max ) )
  m <- as.data.frame( cbind( t, stats$min ) )
  
  # Write to the disk as (European) CSV files for Excel
  write.csv( A, paste0( folder, "/", baseName, exper, "_aggr_avg.csv" ),
             row.names = FALSE, quote = FALSE )
  write.csv( S, paste0( folder, "/", baseName, exper, "_aggr_sd.csv" ),
             row.names = FALSE, quote = FALSE )
  write.csv( M, paste0( folder, "/", baseName, exper, "_aggr_max.csv" ),
             row.names = FALSE, quote = FALSE )
  write.csv( m, paste0( folder, "/", baseName, exper, "_aggr_min.csv" ),
             row.names = FALSE, quote = FALSE )
  
  # Save temporary results to disk to save memory
  tmpFile <- paste0( folder, "/", baseName, exper, datFilSfx )
  save( mc, A, S, M, m, nTsteps, nVar, nSize, file = tmpFile )
  
  return( tmpFile )
}

# only reprocess results file if requested/needed
if( savDat ) {
  
  tmpFiles <- list( )
  noDat <- FALSE
  
  # check all .dat files exist and are newer than .res files
  for( i in 1 : nExp ) {
    tmpFiles[[ i ]] <- paste0( folder, "/", baseName, i, datFilSfx )
    if( ! file.exists( tmpFiles[[ i ]] ) )
      noDat <- TRUE
    else {
      myFiles <- list.files( path = folder, pattern = paste0( baseName, i, "_[0-9]+.res" ),
                             full.names = TRUE )
      # if any .res file is newer, redo everything
      if( length( myFiles ) > 0 &&
          max( file.mtime( myFiles ) ) > file.mtime( tmpFiles[[ i ]] ) ) {
        if( ! noDat )
          cat( "New data files detected, removing previously saved data...\n\n" )
        unlink( tmpFiles[[ i ]] )
        noDat <- TRUE
      }
    }
  }
  
  if( ! noDat )
    cat( "Re-using previously saved data...\n" )
}

if( ! savDat || noDat ) {
  
  cat( "Reading data from files...\n" )
  
  # configure clusters for 2 level parallel loading
  if( cores == 0 )
    cores <- detectCores( )
  cores <- min( cores, detectCores( ) )
  lsdCores <- 1
  if( cores != 1 ) {
    # fully allocate cores (round up to ensure 100% utilization)
    if( cores > nExp )
      lsdCores <- ceiling( cores / nExp )
    
    # initiate cluster for parallel loading
    cl <- makeCluster( min( nExp, cores ) )
    
    # configure cluster: export required variables & packages
    clusterExport( cl, c( "nExp", "folder", "baseName", "aggrVars", "iniDrop",
                          "nKeep", "datFilSfx", "lsdCores" ) )
    invisible( clusterEvalQ( cl, library( LSDinterface ) ) )
    
    # load each experiment in parallel
    tmpFiles <- parLapplyLB( cl, 1 : nExp, readExp )
    
    stopCluster( cl )
    
  } else {
    
    # load each experiment serially
    tmpFiles <- lapply( 1 : nExp, readExp )
  }
}

# ==== Organize data read from files ----

# fill the lists to hold data
mcData <- list() # 3D Monte Carlo data
Adata <- list()  # average data
Sdata <- list()  # standard deviation data
Mdata <- list()  # maximum data
mdata <- list()  # minimum data
nTsteps.1 <- nSize.1 <- 0

for( k in 1 : nExp ) {                      # realocate data in separate lists
  
  load( tmpFiles[[ k ]] )                   # pick data from disk
  if( ! savDat )
    file.remove( tmpFiles[[ k ]] )          # and delete temporary file, if needed
  
  if( k > 1 && ( nTsteps != nTsteps.1 || nSize != nSize.1 ) )
    stop( "Inconsistent data files.\nSame number of time steps and of MC runs is required." )
  
  mcData[[ k ]] <- mc
  rm( mc )
  Adata[[ k ]] <- A
  Sdata[[ k ]] <- S
  Mdata[[ k ]] <- M
  mdata[[ k ]] <- m
  nTsteps.1 <- nTsteps
  nSize.1 <- nSize
}

# free memory
rm( tmpFiles, A, S, M, m, nTsteps.1, nSize.1 )
invisible( gc( verbose = FALSE ) )

#******************************************************************
#
# --------------------- Plot statistics -------------------------
# ===================== User parameters =========================

bCase     <- 1      # experiment to be used as base case
CI        <- 0.95   # desired confidence interval
warmUpPlot<- 200    # number of "warm-up" runs for plots
nTplot    <- 600     # last period to consider for plots (-1=all)
warmUpStat<- 200    # warm-up runs to evaluate all statistics
nTstat    <- 600     # last period to consider for statistics (-1=all)
bPlotCoef <- 1.5    # boxplot whiskers extension from the box (0=extremes)
bPlotNotc <- FALSE  # use boxplot notches
lowP      <- 6      # bandpass filter minimum period
highP     <- 32     # bandpass filter maximum period
bpfK      <- 12     # bandpass filter order
lags      <- 6      # lags to analyze
smoothing <- 1600   # HP filter smoothing factor (lambda)
crisisTh  <- 0.00   # crisis growth threshold
crisisLen <- 3      # crisis minimum duration (periods)
crisisPre <- 4      # pre-crisis period to base trend start (>=1)
crisisRun <- 0      # the crisis case to be plotted (0=auto)
crisesPlt <- TRUE   # plot all the crisis plots in a separate pdf file?

repName   <- "Sectoral_Model"  # report files base name (if "" same baseName)
sDigits   <- 4      # significant digits in tables
transMk   <- -1     # regime transition mark after warm-up (-1:none)
plotRows  <- 1      # number of plots per row in a page
plotCols  <- 1  	  # number of plots per column in a page
plotW     <- 10     # plot window width
plotH     <- 7      # plot window height
raster    <- TRUE  # raster or vector plots
res       <- 600    # resolution of raster mode (in dpi)

# Colors assigned to each experiment's lines in graphics
colors <- c( "black", "blue", "red", "orange", "green", "brown", "yellow", "purple", "gray", "blueviolet" )
#colors <- rep( "black", 6 )

# Line types assigned to each experiment
lTypes <- c( "solid", "solid", "solid", "solid", "solid", "solid", "solid", "solid", "solid", "solid")
#lTypes <- c( "solid", "dashed", "dotted", "dotdash", "longdash", "twodash" )

# Point types assigned to each experiment
pTypes <- c( 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 )
#pTypes <- c( 4, 0, 1, 2, 3, 5 )

# ====== External support functions & definitions ======

library("LSDsensitivity")
source( "support-functions.R" )
source( "sectoral-model-time-plots.R" )
source( "sectoral-model-box-plots.R" )

# ==== Support stuff ====

if( repName == "" )
  repName <- baseName

# Generate fancy labels & build labels list legend
legends <- vector( )
legendList <- "Experiments: "
for( k in 1 : nExp ) {
  if( is.na( expVal[ k ] ) || expVal[ k ] == "" )
    legends[ k ] <- paste( "Case", k )
  else
    legends[ k ] <- expVal[ k ]
  if( k != 1 )
    legendList <- paste0( legendList, ",  " )
  legendList <- paste0( legendList, "[", k, "] ", legends[ k ] )
}

# Number of periods to show in graphics and use in statistics
if( nTplot < 1 || nTplot > nTsteps || nTplot <= warmUpPlot )
  nTplot <- nTsteps
if( nTstat < 1 || nTstat > nTsteps || nTstat <= warmUpStat )
  nTstat <- nTsteps
if( nTstat < ( warmUpStat + 2 * bpfK + 4 ) )
  nTstat <- warmUpStat + 2 * bpfK + 4         # minimum number of periods
TmaxStat <- nTstat - warmUpStat
TmaskPlot <- ( warmUpPlot + 1 ) : nTplot
TmaskStat <- ( warmUpStat + 1 ) : nTstat
TmaskBpf <- ( bpfK + 1 ) : ( TmaxStat - bpfK )

# Calculates the critical correlation limit for significance (under heroic assumptions!)
critCorr <- qnorm( 1 - ( 1 - CI ) / 2 ) / sqrt( nTstat )


# ==== Main code ====

tryCatch({    # enter error handling mode so PDF can be closed in case of error/interruption
  
  # create a daily output directory
  outDir <- format( Sys.time(), "%Y-%m-%d" )
  if( ! dir.exists( paste0( folder, "/", outDir ) ) )
    dir.create( paste0( folder, "/", outDir ) )
  
  cat( paste( "\nSaving results and data to:", paste0( folder, "/", outDir ), "\n" ) )
  
  # Select type of output
  if( raster ){
    # Open PNG (bitmap) files for output
    png( paste0( folder, "/", outDir, "/", repName, "_aggr_plots_%d.png" ),
         width = plotW, height = plotH, units = "in", res = res )
    TRUE
  } else {
    # Open PDF plot file for output
    pdf( paste0( folder, "/", outDir, "/", repName, "_aggr_plots.pdf" ),
         width = plotW, height = plotH )
    par( mfrow = c ( plotRows, plotCols ) )             # define plots per page
    png( paste0( folder, "/", outDir, "/", repName, "_aggr_plots_%d.png" ),
         width = plotW, height = plotH, units = "in", res = res )
  }
  
  #
  # ====== MC PLOTS GENERATION ======
  #
  
  cat( "\nProcessing experiments and generating reports...\n")
  
  time_plots( mcData, Adata, mdata, Mdata, Sdata, nExp, nSize, nTsteps, TmaskPlot,
              CI, legends, colors, lTypes, transMk, smoothing )
  
  if(nExp>1)
  {
  box_plots( mcData, nExp, nSize, TmaxStat, TmaskStat, warmUpStat,
                         nTstat, legends, legendList, sDigits, bPlotCoef,
                         bPlotNotc, folder, outDir, repName )
  }
  
  #
  # ====== STATISTICS GENERATION ======
  #
  
  # Create vectors and lists to hold the Monte Carlo results
  y<-
  y_gr<-          
  infla<-     
  pr<-         
  pcu<- 
  hhi<-
  vector( mode = "numeric", length = nSize )
  
  y_sd<-
  y_gr_sd<-          
  infla_sd<-     
  pr_sd<-         
  pcu_sd<- 
  hhi_sd<-
  vector( mode = "numeric", length = nSize )
  
  y_bpf<-
  y_gr_bpf<-
  infla_bpf<-
  pr_bpf<-
  pcu_bpf<-
  hhi_bpf<-
  vector( mode = 'numeric', length = nSize )
  
  adf_y<-
  adf_y_gr<-
  adf_infla<-
  adf_pr<-
  adf_pcu<-
  adf_hhi<-
  list( )
  
  for(k in 1 : nExp){ # Experiment k
    
    #
    # ---- Bandpass filtered GDP, consumption and investment cycles graphic ----
    #
    
    plot_bpf( list( log0( Adata[[ k ]]$Sector_Income )),
              pl = lowP, pu = highP, nfix = bpfK, mask = TmaskPlot,
              mrk = transMk, # uncomment to add vertical line in selected point (e.g. mark of regime change)
              col = colors, lty = lTypes,
              leg = c( "Income" ),
              xlab = "Time", ylab = "Filtered series",
              tit = paste( "Income cycles (", legends[ k ], ")" ),
              subtit = paste( "( Baxter-King bandpass-filtered, low = 6Q / high = 32Q / order = 12 / MC runs =",
                              nSize, ")" ) )
    
    #
    # ==== Statistics computation for tables ====
    #
    
    for( j in 1 : nSize ){  # Execute for every Monte Carlo run
      
      # Monte carlo average growth rates and shares
      y[ j ]<- mcData[[ k ]][ nTstat, "Sector_Income", j ]
      y_gr[ j ]<- mcData[[ k ]][ nTstat, "Sector_Income_Growth", j ]           
      infla [ j ]<- mcData[[ k ]][ nTstat, "Sector_Inflation", j ]      
      pr [ j ]<- mcData[[ k ]][ nTstat, "Sector_Profit_Rate", j ]            
      pcu [ j ]<- mcData[[ k ]][ nTstat, "Sector_Capacity_Utilization", j ]  
      hhi [ j ]<- mcData[[ k ]][ nTstat, "Sector_Normalized_HHI", j ]
      
      # Apply Baxter-King filter to the series
      y_bpf<- bkfilter( mcData[[ k ]][ TmaskStat, "Sector_Income", j ] , pl = lowP, pu = highP, nfix = bpfK )
      y_gr_bpf<- bkfilter( mcData[[ k ]][ TmaskStat, "Sector_Income_Growth", j ] , pl = lowP, pu = highP, nfix = bpfK )
      infla_bpf<- bkfilter( mcData[[ k ]][ TmaskStat, "Sector_Inflation", j ] , pl = lowP, pu = highP, nfix = bpfK )
      pr_bpf<- bkfilter( mcData[[ k ]][ TmaskStat, "Sector_Profit_Rate", j ] , pl = lowP, pu = highP, nfix = bpfK )
      pcu_bpf<- bkfilter( mcData[[ k ]][ TmaskStat, "Sector_Capacity_Utilization", j ] , pl = lowP, pu = highP, nfix = bpfK )
      hhi_bpf<- bkfilter( mcData[[ k ]][ TmaskStat, "Sector_Normalized_HHI", j ] , pl = lowP, pu = highP, nfix = bpfK )
     
      # Augmented Dickey-Fuller tests for unit roots
      adf_y[[ j ]]<- adf.test( mcData[[ k ]][ TmaskStat, "Sector_Income", j ]  )
      adf_y_gr[[ j ]]<- adf.test( mcData[[ k ]][ TmaskStat, "Sector_Income_Growth", j ]  )
      adf_infla[[ j ]]<- adf.test( mcData[[ k ]][ TmaskStat, "Sector_Inflation", j ]  )
      adf_pr[[ j ]]<- adf.test( mcData[[ k ]][ TmaskStat, "Sector_Profit_Rate", j ]  )
      adf_pcu[[ j ]]<- adf.test( mcData[[ k ]][ TmaskStat, "Sector_Capacity_Utilization", j ]  )
      adf_hhi[[ j ]]<- adf.test( mcData[[ k ]][ TmaskStat, "Sector_Normalized_HHI", j ]  )
      
      # Standard deviations of filtered series
      y_sd[ j ]<- sd( y_bpf$cycle[ TmaskBpf, 1 ] )
      y_gr_sd[ j ]<- sd( y_gr_bpf$cycle[ TmaskBpf, 1 ] )
      infla_sd[ j ]<- sd( infla_bpf$cycle[ TmaskBpf, 1 ] )
      pr_sd[ j ]<- sd( pr_bpf$cycle[ TmaskBpf, 1 ] )
      pcu_sd[ j ]<- sd( pcu_bpf$cycle[ TmaskBpf, 1 ] )
      hhi_sd[ j ]<- sd( hhi_bpf$cycle[ TmaskBpf, 1 ] )
     
    }
    
    #
    # ---- Summary statistics table (averages, standard errors and p-values) ----
    #
    
    key.stats <- matrix(
      c(
        ## avg. growth rate
        mean( y ),
        mean( y_gr ), 
        mean( infla ), 
        mean( pr ), 
        mean( pcu ), 
        mean( hhi ),
        
        ## (Std. Dev.)
        sd( y ),
        sd( y_gr ),
        sd( infla ),
        sd( pr ),
        sd( pcu ),
        sd( hhi ),
        
        ## ADF test (logs)
        mean( unname( sapply( adf_y, `[[`, "statistic" ) ) ),
        mean( unname( sapply( adf_y_gr, `[[`, "statistic" ) ) ),
        mean( unname( sapply( adf_infla, `[[`, "statistic" ) ) ),
        mean( unname( sapply( adf_pr, `[[`, "statistic" ) ) ),
        mean( unname( sapply( adf_pcu, `[[`, "statistic" ) ) ),
        mean( unname( sapply( adf_hhi, `[[`, "statistic" ) ) ),
        
        ## ADF test (logs) p.value
        mean( unname( sapply( adf_y, `[[`, "p.value" ) ) ),
        mean( unname( sapply( adf_y_gr, `[[`, "p.value" ) ) ),
        mean( unname( sapply( adf_infla, `[[`, "p.value" ) ) ),
        mean( unname( sapply( adf_pr, `[[`, "p.value" ) ) ),
        mean( unname( sapply( adf_pcu, `[[`, "p.value" ) ) ),
        mean( unname( sapply( adf_hhi, `[[`, "p.value" ) ) ),
        
        ## S.d. of bpf series
        mean( y_sd ),
        mean( y_gr_sd ), 
        mean( infla_sd ), 
        mean( pr_sd ),
        mean( pcu_sd ), 
        mean( hhi_sd ), 
        
        ## relative s.d. (to Income)
        1, 
        mean( y_gr_sd ) / mean( y_sd ),
        mean( infla_sd ) / mean( y_sd ), 
        mean( pr_sd ) / mean( y_sd ),
        mean( pcu_sd ) / mean( y_sd ), 
        mean( hhi_sd ) / mean( y_sd )
       
      ),
      ncol = 6, byrow = F)
    
    rownames( key.stats ) <- c( "Income",
                                  "Income Growth", 
                                  "Inflation", 
                                  "Profit Rate", 
                                  "Capacity Utilization",
                                  "HHI"
                                  )
    
    colnames( key.stats ) <- c(   "Avg. (Growth) Rate",
                                  "Std. Dev.",
                                  "ADF test (logs)",
                                  "ADF p-val", 
                                  " s.d. (bpf)", 
                                  " s.d. relative to Income" )
    
    textplot( formatC( key.stats, digits = sDigits, format = "g" ), cmar = 2 )
    title <- paste( "Key statistics and unit roots tests for cycles (", legends[ k ], ")" )
    subTitle <- paste( eval( bquote(paste0( "( bpf: Baxter-King bandpass-filtered series, low = ", .( lowP ),
                                            "Q / high = ", .( highP ), "Q / order = ", .( bpfK ),
                                            " / MC runs = ", .( nSize ), " / period = ",
                                            .( warmUpStat + 1 ), " - ", .( nTstat ), " )" ) ) ),
                       eval( bquote( paste0( "( ADF test H0: there are unit roots / non-stationary at ",
                                             .( (1 - CI ) * 100), "% level", " )" ) ) ), sep ="\n" )
    title( main = title, sub = subTitle )
    
    # Write tables to the disk as CSV files for Excel
    write.csv( key.stats , quote = FALSE,
               paste0( folder, "/", outDir, "/", repName, k, "_key_stats.csv" ) )
    
   
    
  }
  cat( "\nDone...\n" )
  
}, interrupt = function( ex ) {
  cat( "An interrupt was detected.\n" )
  print( ex )
  textplot( "Report incomplete due to interrupt." )
}, error = function( ex ) {
  cat( "An error was detected.\n" )
  print( ex )
  textplot( "Report incomplete due to processing error." )
}, finally = {
  options( warn = 0 )
  cat( "\n", as.character( Sys.time( ) ), "-> Releasing resources...\n\n" )
  totalTime <- proc.time( ) - startTime
  print( totalTime )
  # Close PDF plot file
  dev.off( )
} )


