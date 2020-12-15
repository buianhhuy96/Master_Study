import numpy as np
import scipy
import librosa as lb
import librosa.display
import xlwt 
from xlwt import Workbook 
from matplotlib import pyplot as plt
from os import walk

# Calculate distance between 2 audio and store in a matrix using DTW
def dtw(audios, mfcc_audios, output_record):
  
    num_of_files = len(audios)
    D_all = np.zeros((num_of_files,num_of_files))
    for i in range(num_of_files):
        for j in range(num_of_files):         
            # Calculate dtw and store the cost of optimal warping path
            D, path = librosa.sequence.dtw(mfcc_audios[:,:,i], mfcc_audios[:,:,j], metric='cosine')
            print('Comparing audio number: '+str(i)+ ' with audio number: ' + str(j) +'\n')
            D_all[i,j] = D[path[-1, 0], path[-1, 1]]                       
            output_record.write(i+1, j+1, D_all[i,j]) 

    return D_all

# Calculate similarity between 2 audio and store in a matrix 
def cosine_similarity(audios, audio_features, output_record):
    
    num_of_files = len(audios)
    D_all = np.zeros((num_of_files,num_of_files))
    for i in range(num_of_files):
        for j in range(num_of_files):
            D_all[i,j] = np.abs(1-scipy.spatial.distance.cosine(audio_features[:,i], audio_features[:,j]))            
            output_record.write(i+1, j+1, D_all[i,j]) 
    
    return D_all

def main(): 
    
    n_mfcc = 80
    ## path to audio files   
    my_path = 'my_audio'
    
    # Workbook is created 
    wb = Workbook() 
  
    # create sheet to store comparison data. 
    sheet1 = wb.add_sheet('Heatmap All') 

    # read all audio file names into a list
    audios = []
    for (dirpath, dirnames, filenames) in walk(my_path):
        audios.extend(filenames)
        break
    audios.sort(key=lambda x: int(x[0:x.find('_')]))
    
    
    num_of_files = len(audios)
    # heat_map is used to store output of all audios: similarity/distance
    heat_map = np.zeros((num_of_files,num_of_files))
    
    # audio_features is used to store features of audio including mean and std
    audio_features = np.array([[]])
    
    # mfcc_audios is used to store calculated mfcc of each audio files
    mfcc_audios = np.array([[]])
    for i in range(num_of_files):
            # Write file name into output excel
            sheet1.write(i+1, 0, audios[i],style = xlwt.easyxf('font: bold 1'))             
            sheet1.write(0, i+1, audios[i],style = xlwt.easyxf('font: bold 1; alignment: rotation 90')) 
            
            # Read audio and sample rate
            audio_file, fs   = lb.load('./'+my_path +'/'+audios[i], sr=None)
            
            # Calculate mfcc coefficients
            mfcc = (librosa.feature.mfcc(y=audio_file, sr = fs, n_mfcc = n_mfcc ))
            
            # Append calculated mfcc into list of all audios 
            # This will be used later in DTW
            mfcc_audios = np.dstack((mfcc_audios,mfcc)) if mfcc_audios.size else mfcc
            
            # Calculate mean and std of mfcc coefficients
            # This will be used in Cosine Similarity
            mean = np.mean(mfcc, axis = 1)
            std = np.std(mfcc, axis = 1)
            audio_features = np.column_stack([audio_features,np.hstack([mean,std])]) if audio_features.size else np.hstack([mean,std])
            
    # Choose between 2 methods to compare audio files
    # Uncomment one and comment the other so as to choose the method.
    heat_map = cosine_similarity(audios,audio_features,sheet1)
    #heat_map = dtw(audios,mfcc_audios,sheet1)
            
    # Plot the generated heat map
    fig, ax = plt.subplots()
    
    img = ax.imshow(heat_map, cmap='hot', interpolation='nearest')   
    ax.tick_params(labelsize=3)    
    ax.set_yticks(np.linspace(0,num_of_files-1,num_of_files))
    ax.set_xticks(np.linspace(0,num_of_files-1,num_of_files))
    ax.set_yticklabels(audios)
    ax.set_xticklabels(audios,rotation=60,ha='right')
    
    ax.title.set_text('Heat map of all audios')
    

    fig.colorbar(img)    
    plt.tight_layout()
    plt.show()
    
    # Calculate average similarity/distance of the audios
    average = np.mean(heat_map)
    print('\n------ Average value: ' + str(average))
    
    # Save output to reports
    wb.save('Heatmap.xls') 
    fig.savefig('Heatmap_All.jpg',
            format='jpeg',
            dpi=500,
            bbox_inches='tight')
if __name__== "__main__":
    main()







