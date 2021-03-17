using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SocialPlatforms;

public class AuthenticAPI_Test : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        Social.localUser.Authenticate(ProcessAuthentication);
    }


    void ProcessAuthentication(bool success)
    {
        if(success)
        {
            Debug.Log("Authenticated, checking acheivement");

            Social.LoadAchievements(ProcessLoadedAcheivement);
        }
        else
        {
            Debug.Log("Failed to authenticate");
        }
    }

    void ProcessLoadedAcheivement(IAchievement[] achievements)
    {
        if(achievements.Length == 0)
        {
            Debug.Log("Error: no achievement found");
        }
        else
        {
            Debug.Log("Got " + achievements.Length + " achievements");
        }

        Social.ReportProgress("Achievement01", 100.0, result =>
        {
            if (result)
            {
                Debug.Log("Successfully reported achievement progress");
            }
            else
            {
                Debug.Log("Failed to report achievement");
            }
        });
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
