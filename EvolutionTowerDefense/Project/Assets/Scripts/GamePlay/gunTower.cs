﻿using UnityEngine;
using System.Collections;
using System.Linq;

public class gunTower : MonoBehaviour {
	public GameObject endOfBarrel;
	public GameObject smoke;
	private GameObject instantiatedObj;
	private AudioSource shotSound; //Matti change it to private and AudioSource
	public GameObject bullet;


	public float bulletSpeed; //How fast a bullet is shot
	public float fireRate; //How fast a tower fires
	public float fireRadius; //radius that tower detects and fires at enemy

	private Vector3 movementDirection;

	public TowerUpgrader gameController;

	// Use this for initialization
	void Start ()
	{
//		gameController = GetComponent <TowerUpgrader>();

		shotSound = GetComponent<AudioSource> ();//Matti use a simple GetComponent
		fireRate = gameController.GetGunFR();
//		Debug.Log(fireRate);
		fireRadius = gameController.GetGunRange();
//		Debug.Log(fireRadius);

		InvokeRepeating("SpawnBullet",fireRate,fireRate);
	}


	void Update()
	{

		foreach (Collider col in Physics.OverlapSphere (transform.position, fireRadius))
		{

		

			if (col.tag == "Enemy") {
				if (gameObject.tag != "DontRotate") {


					//Attack enemy who is farthest away from turret
					//movementDirection = (col.transform.position - transform.position);


					//Attack enemy who is cloests to turret
					GameObject target = null;
					target	= GameObject.FindGameObjectsWithTag("Enemy").Aggregate((o1, o2) => Vector3.Distance(o1.transform.position, this.transform.position) > Vector3.Distance(o2.transform.position, this.transform.position) ? o2 : o1);

					movementDirection = (target.transform.position - transform.position);
					transform.rotation = Quaternion.LookRotation (movementDirection);
				}

				break;
			}

		}



	}


	public void SpawnBullet()

	{
		/////////////////////////////////
		//Add code here to make it recoil
		//
		//
		//
		//
		//
		////////////////////////////


		GameObject target = null;
		//Loop for each enemy in area - 
		foreach (Collider col in Physics.OverlapSphere (transform.position, fireRadius))
		{

			if (col.tag == "Enemy") {

				//Attack farthest enemy
				//target = col.gameObject;

				//Attack closests enemy
				target = GameObject.FindGameObjectsWithTag("Enemy").Aggregate((o1, o2) => Vector3.Distance(o1.transform.position, this.transform.position) > Vector3.Distance(o2.transform.position, this.transform.position) ? o2 : o1);

				break;
			}
		}

		//This is used to shoot first object anywhere
		//GameObject target = GameObject.FindGameObjectWithTag("Enemy");

		if (target != null) {

			GameObject newBullet = Instantiate (bullet, transform.position, bullet.transform.rotation) as GameObject;
			newBullet.GetComponent<Rigidbody> ().AddForce ((target.transform.position - transform.position).normalized * bulletSpeed, ForceMode.VelocityChange);
		
			//AudioSource.PlayClipAtPoint(shotSound, endOfBarrel.transform.position); //Previous way
			shotSound.Play ();// Matti now simply just do a play
			instantiatedObj= (GameObject) Instantiate(smoke, endOfBarrel.transform.position, endOfBarrel.transform.rotation);
			newBullet.transform.LookAt (target.transform.position);
			Destroy (instantiatedObj,2.0f);
		}
	}

}
