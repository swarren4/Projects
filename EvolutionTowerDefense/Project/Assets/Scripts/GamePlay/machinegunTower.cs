﻿using UnityEngine;
using System.Collections;
using System.Linq;

public class machinegunTower : MonoBehaviour {
	public GameObject endOfBarrel;
	private AudioSource shotSound;
	public GameObject smoke;
	private GameObject instantiatedObj;
	private Quaternion smokeRotation;

	public GameObject bullet;



	public float bulletSpeed = 1.0f; //How fast a bullet is shot
	public float fireRate = 0.0f; //How fast a tower fires
	public float fireRadius= 0.0f; //radius that tower detects and fires at enemy

	public float damage = 1.0f; // Damage


	private Vector3 movementDirection;

	public TowerUpgrader gameController;


	// Use this for initialization
	void Start ()
	{
//		gameController = GetComponent <TowerUpgrader>();
		shotSound = GetComponent<AudioSource> ();

		fireRate = gameController.GetMgFR();
		fireRadius = gameController.GetMgRange();


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
			shotSound.Play ();		
			float xSpin = Random.Range(0,360); 
			float ySpin = Random.Range(0,360);
			float zSpin = Random.Range(0,360);

			instantiatedObj= (GameObject) Instantiate(smoke, endOfBarrel.transform.position, Quaternion.Euler(xSpin, ySpin,zSpin));
			newBullet.transform.LookAt (target.transform.position);

			Destroy (instantiatedObj,2.0f);		
		}
	}

}
